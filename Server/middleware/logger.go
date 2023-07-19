package middleware

import (
	"fmt"
	"os"
	"raccoonserver/models"

	"github.com/gin-gonic/gin"
)

func LogAgentUpload(log *os.File) gin.HandlerFunc {
	return func(c *gin.Context) {
		token := c.Param("token")
		file, _ := c.FormFile("file")
		fmt.Fprintf(log, "[LOGGER] Agent %s uploaded %s\n", token, file.Filename)

		c.Next()
	}
}

func LogAgentRegistration(log *os.File) gin.HandlerFunc {
	return func(c *gin.Context) {
		request := c.MustGet("request").(models.PostRequestRegistration)
		fmt.Fprintf(log, "[LOGGER] New agent registration:  %s\n", request)

		c.Next()
	}
}
