/*
MIT License

Copyright (c) 2023 ISSuh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

package option

import (
	"errors"
	"io/ioutil"
	"strconv"

	"github.com/sirupsen/logrus"
	"gopkg.in/yaml.v2"
)

const (
	BUnit  = "B"
	KBUnit = "K"
	MBUnit = "M"
	GBUnit = "G"
)

func LoadOptionFile(path string, option interface{}) error {
	if len(path) <= 0 {
		return errors.New("Invalid option file path")
	}

	var buffer []byte
	var err error
	if buffer, err = loadFile(path); err != nil {
		return err
	}

	logrus.Infoln("\n", string(buffer))
	if err = yaml.Unmarshal(buffer, option); err != nil {
		return err
	}
	return nil
}

func ParseCapacityUnit(valueWithUnit string) int {
	valueStr := ""
	unitStr := valueWithUnit[len(valueWithUnit)-1:]
	unit := 1

	switch unitStr {
	case KBUnit:
		unit *= KB
		valueStr = valueWithUnit[:len(valueWithUnit)-1]
	case MBUnit:
		unit *= MB
		valueStr = valueWithUnit[:len(valueWithUnit)-1]
	case GBUnit:
		unit *= GB
		valueStr = valueWithUnit[:len(valueWithUnit)-1]
	default:
		unit = 1
		valueStr = valueWithUnit
	}

	value, err := strconv.Atoi(valueStr)
	if err != nil {
		logrus.Errorln("ParseCapacityUnit - invalid value. ", valueWithUnit)
		return 0
	}
	return value * unit
}

func loadFile(path string) ([]byte, error) {
	buf, err := ioutil.ReadFile(path)
	if err != nil {
		return nil, err
	}
	return buf, nil
}
