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

package storage

import (
	"errors"

	"github.com/ISSuh/lsm-tree/storage"
	"github.com/ISSuh/simpledb/internal/option"
)

type LsmStorageStorage struct {
	engine *storage.Storage
}

func NewLsmStorageStorage() *LsmStorageStorage {
	return &LsmStorageStorage{
		engine: nil,
	}
}

func (lss *LsmStorageStorage) Open(option option.SimpleDbOption) error {
	storageOption := lss.parseOption(option)
	lss.engine = storage.NewStorage(storageOption)
	if lss.engine == nil {
		return errors.New("can not create storage engine")
	}
	return nil
}

func (lss *LsmStorageStorage) Put(key string, value []byte) error {
	if lss.engine == nil {
		return errors.New("storage engine not opend")
	}
	return nil
}

func (lss *LsmStorageStorage) Get(key string) ([]byte, error) {
	if lss.engine == nil {
		return nil, errors.New("storage engine not opend")
	}
	return lss.engine.Get(key), nil
}

func (lss *LsmStorageStorage) Remove(key string) error {
	if lss.engine == nil {
		return errors.New("storage engine not opend")
	}
	lss.engine.Remove(key)
	return nil
}

func (lss *LsmStorageStorage) parseOption(option option.SimpleDbOption) storage.Option {
	var storageOptions storage.Option
	storageOptions.Path = option.Path
	storageOptions.BlockSize = option.BlockSize
	storageOptions.TableSize = option.TableSize
	storageOptions.LimitedFilesNumOnL0 = option.LimitedFilesNumOnL0
	storageOptions.TableSizeOffset = option.TableSizeOffset
	storageOptions.LevelOnSkipList = option.LevelOnSkipList
	storageOptions.MemTableSize = option.MemTableSize
	return storageOptions
}
