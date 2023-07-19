package middleware

import (
	"net/http"
	"raccoonserver/database"

	"github.com/gin-gonic/gin"
)

func Authenticator() gin.HandlerFunc {
	return func(c *gin.Context) {
		token := c.Param("token")
		if isValid, _ := database.IsTokenValid(token); isValid { // hey, thats neat!
			c.Next()
		} else {
			c.AbortWithStatus(http.StatusNoContent)
		}

	}
}
