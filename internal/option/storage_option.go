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

const (
	_      = iota
	KB int = 1 << (10 * iota)
	MB
	GB
)

const (
	defaultLevel               = 7
	defaultBlockSize           = 4
	defaultTableSize           = 2 * MB
	defaultLimitedFilesNumOnL0 = 2
	defaultTableSizeOffset     = 10
	defaultLevelOnSkipList     = 5
	defaultMemTableSize        = 4 * MB
)

type StorageOption struct {
	// default directory path where placed files
	Path string `yaml:"path"`

	// max level of files
	// start level is 0
	// the last level has unlimit number of files
	Level int `yaml:"level"`

	// max block size on table
	BlockSize int `yaml:"block_size"`

	// max L0 table size
	TableSize int `yaml:"table_size"`

	// limited number of files on level
	// key is level, value is limited number
	// the last level has unlimit number of files
	LimitedFilesNumOnL0 int `yaml:"libited_file_num_on_l0"`

	// offset of calculated table size whne inscrease level
	// if tableSize value is 10Mb and tableSizeOffset value is 10,
	// the Max L0 file size is 10Mb and L1 file size is 100Mb(10Mb * 10)
	TableSizeOffset int `yaml:"table_size_offset"`

	// max level value on skiplist
	LevelOnSkipList int `yaml:"level_on_skiplist"`

	// limited number of memtable
	MemTableSize int `yaml:"memtable_size"`
}

func NewStorageOption(path string) StorageOption {
	return StorageOption{
		Path:                path,
		Level:               defaultLevel,
		BlockSize:           defaultBlockSize,
		TableSize:           defaultTableSize,
		LimitedFilesNumOnL0: defaultTableSize,
		TableSizeOffset:     defaultTableSizeOffset,
		LevelOnSkipList:     defaultLevelOnSkipList,
		MemTableSize:        defaultMemTableSize,
	}
}
