/*
 * ConverterSmbToNFDETest.h
 *
 *  Created on: Jan 16, 2015
 *      Author: luis
 */

#ifndef SRC_TEST_UTILS_SMBTONFDE_ADAPTERFDTDTEST_H_
#define SRC_TEST_UTILS_SMBTONFDE_ADAPTERFDTDTEST_H_

#include "gtest/gtest.h"
#include "../../parser/ParserGiDTest.h"
#include "adapter/fdtd/AdapterFDTD.h"
#include "exporter/gid/OutputGiD.h"
#include "exporter/nfde/OutputNFDE.h"


class AdapterFDTDTest :
public ::ParserGiDTest,
public ::testing::WithParamInterface<const char*> {

protected:
    AdapterFDTDTest() {

    }

    void compare(const SmbData* smb, const SmbData* nfde) const;
    void runProject(const SmbData* smb) const;
};

#endif /* SRC_TEST_UTILS_SMBTONFDE_ADAPTERFDTDTEST_H_ */
