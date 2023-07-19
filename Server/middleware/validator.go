package middleware

import (
	"errors"
	"net/http"
	"raccoonserver/configs"
	"raccoonserver/models"

	"github.com/gin-gonic/gin"
	"github.com/gin-gonic/gin/binding"
)

func ValidateRegistration(config *configs.ServerConfig) gin.HandlerFunc {
	return func(c *gin.Context) {

		var request models.PostRequestRegistration

		// accept arbitrary content-type's via an overwrite
		//contentType := c.Request.Header.Get("Content-Type")
		//println(contentType)
		contentType := "application/x-www-form-urlencoded"
		c.Request.Header.Set("Content-Type", contentType)

		// Bind the raw request body to the PostRequestRegistration struct
		if err := c.MustBindWith(&request, binding.Form); err != nil {
			c.AbortWithStatus(http.StatusNoContent)
			return
		}

		// Validate the PostRequestRegistration struct
		if err := validatePostRequest(request, config); err != nil {
			c.AbortWithStatus(http.StatusNoContent)
			return
		}

		// store PostRequestRegistration object on the context and move to next handler
		c.Set("request", request)
		c.Next()
	}
}

func validatePostRequest(request models.PostRequestRegistration, config *configs.ServerConfig) error {
	// Check that the required fields are present
	if request.MachineId == "" {
		return errors.New("MachineId is required")
	}
	if request.ConfigID != config.ConfigID {
		return errors.New("ConfigId is required")
	}

	// Additional validation logic can go here...
	// Should validate no '|' injections
	// Should validate the guid format

	// If all validation checks pass, return nil
	return nil
}
