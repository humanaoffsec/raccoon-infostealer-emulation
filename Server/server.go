package main

import (
	"bufio"
	"context"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/signal"
	"raccoonserver/configs"
	"raccoonserver/database"
	"raccoonserver/routers"
	"strings"
	"syscall"
)

func main() {

	// Generate configuration object
	config, err := configs.LoadServerConfig()
	if err != nil {
		log.Fatalf("Failed to parse config.toml: %v", err)
	}

	// Initialize a new DB (wiping out the old one!)
	db, err := database.NewDB(config)
	if err != nil {
		log.Fatalf("DB not initialized: %v", err)
	}
	defer db.Close()

	// Initialize the router
	router := routers.SetupRouter(config)

	// Create an HTTP server using the Gin router
	server := &http.Server{
		Addr:    ":80",
		Handler: router,
	}

	// Start the server in a separate goroutine
	go func() {
		if err := server.ListenAndServe(); err != nil && err != http.ErrServerClosed {
			log.Fatalf("Failed to start server: %v", err)
		}
	}()

	// Create a channel to listen for signals from the command line
	sigCh := make(chan os.Signal, 1)
	signal.Notify(sigCh, syscall.SIGINT, syscall.SIGTERM, os.Interrupt)

	// Create a scanner to read user input from the terminal
	scanner := bufio.NewScanner(os.Stdin)

	// Start a goroutine to prompt the user for input
	// This is a hack job, you have been warned...
	go func() {
		for {
			fmt.Print("> ")
			if !scanner.Scan() {
				return
			}
			input := scanner.Text()

			// Split the input into function name and arguments
			args := strings.Split(input, " ")
			if len(args) == 0 {
				continue
			}
			functionName := args[0]
			functionArgs := args[1:]

			// Call the specified function with the provided arguments
			switch functionName {
			case "saveFiles":
				if len(functionArgs) != 2 {
					fmt.Println("Invalid arguments for saveFiles function. Usage: saveFiles <token> <directory>")
					continue
				}
				token := functionArgs[0]
				directory := functionArgs[1]
				database.SaveAgentUploads(token, directory)
			case "help":
				fmt.Println("saveFiles <token> <directory>")
			default:
				continue
			}
		}
	}()

	// Listen for signals
	<-sigCh
	os.Stdin.Close()
	fmt.Println("\nShutting down server...")
	// Shutdown the API server gracefully
	if err := server.Shutdown(context.Background()); err != nil {
		fmt.Println("Error shutting down API server:", err)
	}
}
