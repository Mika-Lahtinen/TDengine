/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TSDB_FILE_SYSTEM_H
#define _TSDB_FILE_SYSTEM_H

#include "tsdb.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exposed Handle */
struct STFileSystem;

/* Exposed APIs */
// open/close
int32_t tsdbOpenFileSystem(STsdb *pTsdb, struct STFileSystem **ppFS, int8_t rollback);
int32_t tsdbCloseFileSystem(struct STFileSystem **ppFS);
// txn
// int32_t tsdb

/* Exposed Structs */
struct STFileSystem {
  STsdb           *pTsdb;
  int32_t          nFileSet;
  struct SFileSet *aFileSet;
};

#ifdef __cplusplus
}
#endif

#endif /*_TSDB_FILE_SYSTEM_H*/