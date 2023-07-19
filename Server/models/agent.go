package models

type Agent struct {
	ID          int
	MachineGuid string
	Username    string
	Token       string
}

type PostRequestRegistration struct {
	MachineId string `form:"machineId" binding:"required"`
	ConfigID  string `form:"configId" binding:"required"`
}
