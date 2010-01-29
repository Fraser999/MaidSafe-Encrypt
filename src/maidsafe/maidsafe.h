/*
* ============================================================================
*
* Copyright [2009] maidsafe.net limited
*
* Description:  Definition of system-wide constants/enums/structs
* Version:      1.0
* Created:      2009-01-29-00.15.50
* Revision:     none
* Compiler:     gcc
* Author:       Fraser Hutchison (fh), fraser.hutchison@maidsafe.net
* Company:      maidsafe.net limited
*
* The following source code is property of maidsafe.net limited and is not
* meant for external use.  The use of this code is governed by the license
* file LICENSE.TXT found in the root of this directory and also on
* www.maidsafe.net.
*
* You are not free to copy, amend or otherwise use this source code without
* the explicit written permission of the board of directors of maidsafe.net.
*
* ============================================================================
*/

#ifndef MAIDSAFE_MAIDSAFE_H_
#define MAIDSAFE_MAIDSAFE_H_

#include <boost/cstdint.hpp>
#include <maidsafe/maidsafe-dht.h>
#define THIS_MAIDSAFE_DHT_VERSION 15
#if MAIDSAFE_DHT_VERSION < THIS_MAIDSAFE_DHT_VERSION
#error This API is not compatible with the installed library.
#error Please update the maidsafe-dht library.
#elif MAIDSAFE_DHT_VERSION > THIS_MAIDSAFE_DHT_VERSION
#error This API uses a newer version of the maidsafe-dht library.
#error Please update this project.
#endif
#include <stdint.h>

#include <string>

#include "maidsafe/returncodes.h"
#include "protobuf/datamaps.pb.h"
#include "protobuf/packet.pb.h"

// system constants
const boost::uint32_t kMinRegularFileSize = 512;
// This is the size in bytes of the NON-HEX format strings used as keys.  When
// encoded to hex the string size is doubled.
const boost::uint32_t kKeySize = 64;
// const crypto::hashtype kHashSize(crypto::SHA_512);

typedef boost::function<void (const int&)> VoidFuncOneInt;

struct Key_Type {
  Key_Type() : package_type(), id(), private_key(), public_key() {}
  maidsafe::PacketType package_type;
  std::string id;
  std::string private_key;
  std::string public_key;
};

struct bufferpacket_messages {
  bufferpacket_messages() : index(), message(), sender(), type() {}
  std::string index;
  std::string message;
  std::string sender;
  maidsafe::MessageType type;
};

//  enum BufferPacketType {
//    MPID, MAID, PMID
//  };

enum MaidsafeRpcResult {
  kNack, kAck, kNotRemote, kBusy
};

const std::string kAnonymousRequestSignature(2 * kKeySize, 'f');

enum DbInitFlag {CONNECT, CREATE, DISCONNECT};

// struct seh_processed_jobs {
//   std::string path;
//   int exit_code;
//   seh_job_type type;
// };

const std::string kRoot("/");
const std::string kKeysDb("/KeysDb");

const int kRootSubdirSize = 2;
const int kSharesSubdirSize = 1;

const std::string kRootSubdir[kRootSubdirSize][2] = {
  {"/My Files", ""},
  {"/Shares", "c7e625436063a42719208d02ff2bc12498502fd04240d64a4c8b5c8aafb3362e"
              "d2302ee117394fb06d291b78dd0195dcb9f371c3806732bdf872b46923079bc8"
  }
};

const std::string kSharesSubdir[kSharesSubdirSize][2] = {
  {"/Shares/Private", ""} /*,*/
//  {
//  "/Shares/Public", "a0590baf0f811834de68fec77950c179595f5ecb5dc3c6abac67dc34
//  "9714101e40b44531054196b4616f3314cee94d71babb5fbc7010d7fff958d8c8cc54836c"
//  },
//  {
//    "/Shares/Anonymous",
//    "63ed99cc9f91c7dd568247337fd5b479e2cec00e9054ec4c5797c3"
//    "19a80fe3ab07a01dca8200dfd63142b1ed376970bb3a9acd3fa55e9"
//    "d631d3c0aff42f7660e"
//  }
};

const std::string kAccount("ACCOUNT");

// const std::string default_dir_[] = {
//   "/Documents",
//   "/Backup",
//   "/Groups",
//   "/Library",
//   "/Maidsafe",
//   "/Music",
//   "/Pictures",
//   "/Public",
//   "/Sites",
//   "/Software",
//   "/Web"
// };
//
const std::string no_compress_type[] =  {
  ".jpg",
  ".jpeg",
  ".jpe",
  ".jfif",
  ".gif",
  ".png",
  ".mp3",
  ".mp4",
  ".0",
  ".000",
  ".7z",
  ".ace",
  ".ain",
  ".alz",
  ".apz",
  ".ar",
  ".arc",
  ".ari",
  ".arj",
  ".axx",
  ".ba",
  ".bh",
  ".bhx",
  ".boo",
  ".bz",
  ".bz2",
  ".bzip2",
  ".c00",
  ".c01",
  ".c02",
  ".car",
  ".cbr",
  ".cbz",
  ".cp9",
  ".cpgz",
  ".cpt",
  ".dar",
  ".dd",
  ".deb",
  ".dgc",
  ".dist",
  ".ecs",
  ".efw",
  ".fdp",
  ".gca",
  ".gz",
  ".gzi",
  ".gzip",
  ".ha",
  ".hbc",
  ".hbc2",
  ".hbe",
  ".hki",
  ".hki1",
  ".hki2",
  ".hki3",
  ".hpk",
  ".hyp",
  ".ice",
  ".ipg",
  ".ipk",
  ".ish",
  ".j",
  ".jgz",
  ".jic",
  ".kgb",
  ".lbr",
  ".lha",
  ".lnx",
  ".lqr",
  ".lzh",
  ".lzm",
  ".lzma",
  ".lzo",
  ".lzx",
  ".md",
  ".mint",
  ".mpkg",
  ".mzp",
  ".p7m",
  ".package",
  ".pae",
  ".pak",
  ".paq6",
  ".paq7",
  ".paq8",
  ".par",
  ".par2",
  ".pbi",
  ".pcv",
  ".pea",
  ".pf",
  ".pim",
  ".pit",
  ".piz",
  ".pkg",
  ".pup",
  ".puz",
  ".pwa",
  ".qda",
  ".r00",
  ".r01",
  ".r02",
  ".r03",
  ".rar",
  ".rev",
  ".rk",
  ".rnc",
  ".rpm",
  ".rte",
  ".rz",
  ".rzs",
  ".s00",
  ".s01",
  ".s02",
  ".s7z",
  ".sar",
  ".sdn",
  ".sea",
  ".sen",
  ".sfs",
  ".sfx",
  ".sh",
  ".shar",
  ".shk",
  ".shr",
  ".sit",
  ".sitx",
  ".spt",
  ".sqx",
  ".sqz",
  ".tar",
  ".tbz2",
  ".tgz",
  ".tlz",
  ".uc2",
  ".uha",
  ".vsi",
  ".wad",
  ".war",
  ".wot",
  ".xef",
  ".xez",
  ".xpi",
  ".xx",
  ".y",
  ".yz",
  ".z",
  ".zap",
  ".zfsendtotarget",
  ".zip",
  ".zix",
  ".zoo",
  ".zz"
};

// config file name
const std::string kConfigFileName("maidsafe.cfg");
const int kMaxPort = 65535;
const int kMinPort = 5000;

const int kValidityCheckMinTime(1800);  // 30 min
const int kValidityCheckMaxTime(86400);  // 24 hours
// frequency to execute validity check process
const int kValidityCheckInterval(120);  // 2 minutes
// delay to check partner references
const int kCheckPartnerRefDelay(300);  // 5 minutes
// timeout for account amendment transactions in milliseconds
const boost::uint64_t kAccountAmendmentTimeout(120000);
// max. no. of account amendments
const size_t kMaxAccountAmendments(1000);
// max. no. of repeated account amendments (i.e. for same chunk to same PMID)
const size_t kMaxRepeatedAccountAmendments(10);
const int kValidityCheckRetry(2);  // retries for validity check (timeouts)
const boost::uint8_t kMinChunkCopies(4);
const int kMaxChunkLoadRetries(3);  // max number of tries to load a chunk
const int kMaxChunkStoreTries(2);  // max number of tries to store or update a
                                   // chunk
const boost::uint8_t kMaxStoreFailures(10);  // max number of failed store tries
const boost::uint32_t kSaveUpdatesTrigger(10);  // max no of dbs in save queue
                                                 // before running save queue
const double kMinSuccessfulPecentageOfUpdating(0.9);
// max. no. of threads in chunk_thread_pool_ (in MaidsafeStoreManager)
const int kChunkMaxThreadCount(20);
// max. no. of threads in packet_thread_pool_ (in MaidsafeStoreManager)
const int kPacketMaxThreadCount(10);
// port where the service to register a local vault is listening
const boost::uint16_t kLocalPort = 5484;
// additionally paying PMIDs kept in watch lists
const int kMaxReserveWatchListEntries = 250;
// time a watcher is kept in the ChunkInfoHandler's waiting list
const int kChunkInfoWatcherPendingTimeout = 86400;  // 24 hours
// time until a chunk holder is not considered active anymore
const int kChunkInfoRefActiveTimeout = 86400;  // 24 hours
// min. no. of majority of responses from group of k nodes to accept result
const int kKadTrustThreshold(3);

namespace maidsafe {

enum DirType {ANONYMOUS, PRIVATE, PRIVATE_SHARE, PUBLIC_SHARE};

enum ValueType {
  SYSTEM_PACKET, BUFFER_PACKET, BUFFER_PACKET_INFO, BUFFER_PACKET_MESSAGE,
  CHUNK_REFERENCE, WATCH_LIST, DATA, PDDIR_SIGNED, PDDIR_NOTSIGNED
};

enum SortingMode { ALPHA, RANK, LAST };
enum ShareFilter { kAll, kRo, kAdmin };

}  // namespace maidsafe

inline std::string HexSubstr(const std::string &non_hex) {
  return (base::EncodeToHex(non_hex).substr(0, 10) + "...");
}

#endif  // MAIDSAFE_MAIDSAFE_H_
