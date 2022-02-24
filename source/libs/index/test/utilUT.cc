#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "index.h"
#include "indexInt.h"
#include "index_cache.h"
#include "index_fst.h"
#include "index_fst_counting_writer.h"
#include "index_fst_util.h"
#include "index_tfile.h"
#include "index_util.h"
#include "tglobal.h"
#include "tskiplist.h"
#include "tutil.h"

class UtilEnv : public ::testing::Test {
 protected:
  virtual void SetUp() {
    src = (SArray *)taosArrayInit(2, sizeof(void *));
    for (int i = 0; i < 3; i++) {
      SArray *m = taosArrayInit(10, sizeof(uint64_t));
      taosArrayPush(src, &m);
    }

    rslt = (SArray *)taosArrayInit(10, sizeof(uint64_t));
  }
  virtual void TearDown() {
    for (int i = 0; i < taosArrayGetSize(src); i++) {
      SArray *m = (SArray *)taosArrayGetP(src, i);
      taosArrayDestroy(m);
    }
    taosArrayDestroy(src);
  }

  SArray *src;
  SArray *rslt;
};

static void clearSourceArray(SArray *p) {
  for (int i = 0; i < taosArrayGetSize(p); i++) {
    SArray *m = (SArray *)taosArrayGetP(p, i);
    taosArrayClear(m);
  }
}
static void clearFinalArray(SArray *p) { taosArrayClear(p); }
TEST_F(UtilEnv, intersectionSimpleResult) {
  SArray *f = (SArray *)taosArrayGetP(src, 0);
  for (int i = 0; i < 10; i++) {
    uint64_t val = i;
    taosArrayPush(f, &val);
  }
  f = (SArray *)taosArrayGetP(src, 1);
  for (int i = 0; i < 10; i++) {
    uint64_t val = i;
    taosArrayPush(f, &val);
  }
  f = (SArray *)taosArrayGetP(src, 2);
  for (int i = 0; i < 10; i++) {
    uint64_t val = i;
    taosArrayPush(f, &val);
  }
  iIntersection(src, rslt);
  assert(taosArrayGetSize(rslt) == 10);

  clearSourceArray(src);
  clearFinalArray(rslt);
}
TEST_F(UtilEnv, intersectMultiEmptyResult) {
  SArray *f = (SArray *)taosArrayGetP(src, 0);
  for (int i = 10; i < 20; i++) {
    uint64_t val = i;
    taosArrayPush(f, &val);
  }
  f = (SArray *)taosArrayGetP(src, 1);
  for (int i = 0; i < 10; i++) {
    uint64_t val = i;
    taosArrayPush(f, &val);
  }
  f = (SArray *)taosArrayGetP(src, 2);
  for (int i = 20; i < 30; i++) {
    uint64_t val = i;
    taosArrayPush(f, &val);
  }
  // empty source
  iIntersection(src, rslt);
  assert(taosArrayGetSize(rslt) == 0);
  clearSourceArray(src);
  clearFinalArray(rslt);
}
TEST_F(UtilEnv, intersectSimpleEmpty) {
  clearSourceArray(src);
  clearFinalArray(rslt);

  iIntersection(src, rslt);
  assert(taosArrayGetSize(rslt) == 0);
}
TEST_F(UtilEnv, intersect01) {
  clearSourceArray(src);
  clearFinalArray(rslt);

  uint64_t arr1[] = {2, 3, 4, 5};
  SArray * f = (SArray *)taosArrayGetP(src, 0);
  for (int i = 0; i < sizeof(arr1) / sizeof(arr1[0]); i++) {
    taosArrayPush(f, &arr1[i]);
  }

  uint64_t arr2[] = {1, 2, 3, 5};
  f = (SArray *)taosArrayGetP(src, 1);
  for (int i = 0; i < sizeof(arr2) / sizeof(arr2[0]); i++) {
    taosArrayPush(f, &arr2[i]);
  }

  uint64_t arr3[] = {3, 5, 10, 11};
  f = (SArray *)taosArrayGetP(src, 2);
  for (int i = 0; i < sizeof(arr3) / sizeof(arr3[0]); i++) {
    taosArrayPush(f, &arr3[i]);
  }

  iIntersection(src, rslt);
  assert(taosArrayGetSize(rslt) == 2);
}
TEST_F(UtilEnv, intersect02) {
  clearSourceArray(src);
  clearFinalArray(rslt);

  uint64_t arr1[] = {13, 14, 15};
  SArray * f = (SArray *)taosArrayGetP(src, 0);
  for (int i = 0; i < sizeof(arr1) / sizeof(arr1[0]); i++) {
    taosArrayPush(f, &arr1[i]);
  }

  uint64_t arr2[] = {8, 10, 12, 13};
  f = (SArray *)taosArrayGetP(src, 1);
  for (int i = 0; i < sizeof(arr2) / sizeof(arr2[0]); i++) {
    taosArrayPush(f, &arr2[i]);
  }

  uint64_t arr3[] = {9, 10, 11};
  f = (SArray *)taosArrayGetP(src, 2);
  for (int i = 0; i < sizeof(arr3) / sizeof(arr3[0]); i++) {
    taosArrayPush(f, &arr3[i]);
  }

  iIntersection(src, rslt);
  assert(taosArrayGetSize(rslt) == 0);
}
