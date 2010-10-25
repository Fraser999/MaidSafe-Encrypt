/*
* ============================================================================
*
* Copyright [2009] maidsafe.net limited
*
* Description:  Factory for system signature packets
* Version:      1.0
* Created:      2009-01-29-00.23.23
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

#ifndef MAIDSAFE_CLIENT_PACKETFACTORY_H_
#define MAIDSAFE_CLIENT_PACKETFACTORY_H_

#include <boost/any.hpp>
#include <gtest/gtest_prod.h>
#include <maidsafe/base/crypto.h>
#include <map>
#include <string>

#include "protobuf/datamaps.pb.h"

namespace maidsafe {

const boost::uint16_t kRsaKeySize = 4096;  // size to generate RSA keys in bits.
const boost::uint16_t kNoOfSystemPackets = 8;

typedef std::map<std::string, boost::any> PacketParams;

class Packet {
 public:
  Packet();
  virtual ~Packet();
  virtual PacketParams Create(PacketParams params) = 0;
  virtual PacketParams GetData(const std::string &ser_packet,
      PacketParams params) = 0;
  virtual std::string PacketName(PacketParams params) = 0;
 protected:
  crypto::Crypto crypto_obj_;
  virtual bool ValidateSignature(const GenericPacket &packet,
      const std::string &public_key);
 private:
  Packet &operator=(const Packet&);
  Packet(const Packet&);
};

class PacketFactory {
 public:
  static boost::shared_ptr<Packet> Factory(const PacketType type);
 private:
  PacketFactory &operator=(const PacketFactory&);
  PacketFactory(const PacketFactory&);
};

}  // namespace maidsafe

#endif  // MAIDSAFE_CLIENT_PACKETFACTORY_H_
