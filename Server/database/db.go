package database

import (
	"bytes"
	"crypto/rand"
	"database/sql"
	"encoding/hex"
	"fmt"
	"io"
	"log"
	"mime/multipart"
	"os"
	"raccoonserver/configs"
	"raccoonserver/models"
	"strconv"

	_ "github.com/mattn/go-sqlite3"
)

// DB is the global database connection.
var DB *sql.DB

// NewDB returns a new database connection.
func NewDB(config *configs.ServerConfig) (*sql.DB, error) {
	// For dev reasons, wipe the db if it exists.
	removeIfExists(config.DbPath)

	// open DB
	db, err := sql.Open("sqlite3", config.DbPath)
	if err != nil {
		return nil, err
	}

	// If no DB, init a new one
	// for now, always init a new one.
	initDB(config, db)

	// Set the global database connection.
	DB = db

	return db, nil
}

func removeIfExists(filepath string) {
	// Check if the file exists
	if _, err := os.Stat(filepath); os.IsNotExist(err) {
		fmt.Println("db.go: File does not exist.")
		return
	}

	// Remove the file
	err := os.Remove(filepath)
	if err != nil {
		fmt.Println(err)
		return
	}
}

// Initialize a newly created db
func initDB(config *configs.ServerConfig, db *sql.DB) {
	fmt.Println("db.go: initializing DB...")
	_, err := db.Exec(`
		CREATE TABLE IF NOT EXISTS  agents (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			machineGuid TEXT NOT NULL,
			username TEXT,
			token TEXT UNIQUE
		);
		CREATE TABLE IF NOT EXISTS  files (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			agent_id INTEGER NOT NULL,
			name TEXT NOT NULL,
			content BLOB NOT NULL,
			FOREIGN KEY (agent_id) REFERENCES agents (id) ON DELETE CASCADE ON UPDATE CASCADE
		);
	`)

	if err != nil {
		log.Fatal(err)
	}
}

// InsertAgent creates a new agent in the database.
func InsertAgent(agent *models.Agent) error {
	// Generate uniqe token and assign it to the agent
	var err error
	agent.Token, err = generateUniqueToken(DB)
	if err != nil {
		return err
	}

	// Prepare query
	stmt, err := DB.Prepare("INSERT INTO agents (machineGuid, username, token) values(?, ?, ?)")
	if err != nil {
		return err
	}
	defer stmt.Close()

	// exec query
	_, err = stmt.Exec(agent.MachineGuid, agent.Username, agent.Token)
	if err != nil {
		return err
	}

	return nil
}

func generateUniqueToken(db *sql.DB) (string, error) {
	for {
		// Generate a new token.
		bytes := make([]byte, 16)
		_, err := rand.Read(bytes)
		if err != nil {
			return "", err
		}
		token := hex.EncodeToString(bytes)

		// Check if the token already exists in the database.
		var count int
		err = db.QueryRow("SELECT COUNT(*) FROM agents WHERE token = ?", token).Scan(&count)
		if err != nil {
			return "", err
		}
		if count == 0 {
			// Token is unique, return it.
			return token, nil
		}
	}
}

func IsTokenValid(token string) (bool, error) {
	// Check if token is in database
	var count int
	err := DB.QueryRow("SELECT COUNT(*) FROM agents WHERE token = ?", token).Scan(&count)
	if err != nil {
		// error reading db, return that error + false.
		return false, err
	}
	if count == 0 {
		// Token is not in database, return false.
		return false, nil
	} else {
		// Token is in database, return true.
		return true, nil
	}
}

func StoreFile(token string, file *multipart.FileHeader) error {
	// Read file content
	f, _ := file.Open()
	defer f.Close()
	buf := bytes.NewBuffer(nil)
	if _, err := io.Copy(buf, f); err != nil {
		return err
	}

	// Get the agent_id corresponding to the token
	var agentID int
	err := DB.QueryRow("SELECT id FROM agents WHERE token = ?", token).Scan(&agentID)
	if err != nil {
		return err
	}

	// Prepare insert
	stmt, err := DB.Prepare("INSERT INTO files (name, content, agent_id) values(?, ?, ?)")
	if err != nil {
		return err
	}
	defer stmt.Close()

	// exec insert
	_, err = stmt.Exec(file.Filename, buf.Bytes(), agentID)
	if err != nil {
		return err
	}
	return nil
}

// TODO: when the user puts a path that doesn't exist, it never works again?
func SaveAgentUploads(token string, directory string) error {
	// Get the agent_id corresponding to the token
	var agentID int
	err := DB.QueryRow("SELECT id FROM agents WHERE token = ?", token).Scan(&agentID)
	if err != nil {
		return err
	}

	// Query the database to retrieve all files uploaded by the agent
	rows, err := DB.Query("SELECT id, name, content FROM files WHERE agent_id = ?", agentID)
	if err != nil {
		return err
	}
	defer rows.Close()

	// Iterate over the rows and save each file to disk
	for rows.Next() {
		var id int
		var name string
		var content []byte
		if err := rows.Scan(&id, &name, &content); err != nil {
			return err
		}

		// Write the content of the file to disk
		file, err := os.Create(directory + "/" + strconv.Itoa(id) + "_" + name)
		if err != nil {
			return err
		}
		defer file.Close()

		if _, err := io.Copy(file, bytes.NewReader(content)); err != nil {
			return err
		}
	}

	if err := rows.Err(); err != nil {
		return err
	}

	return nil
}
