/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CPP_FILECOIN_SYNC_BLOCKSYNC_COMMON_HPP
#define CPP_FILECOIN_SYNC_BLOCKSYNC_COMMON_HPP

#include "codec/cbor/cbor.hpp"

#include "common.hpp"

namespace fc::storage::ipfs {
  class IpfsDataStore;
}

namespace fc::sync::blocksync {

  enum RequestOptions {
    BLOCKS_ONLY = 0,
    MESSAGES_ONLY = 1,
    BLOCKS_AND_MESSAGES = 2,
  };

  struct Request {
    std::vector<CID> block_cids;
    uint64_t depth = 1;
    RequestOptions options = BLOCKS_AND_MESSAGES;
  };

  using MsgIncudes = std::vector<std::vector<uint64_t>>;

  struct TipsetBundle {
    // TODO use not so heap consuming containers, like small_vector

    std::vector<BlockHeader> blocks;
    std::vector<SignedMessage> secp_msgs;
    MsgIncudes secp_msg_includes;
    std::vector<UnsignedMessage> bls_msgs;
    MsgIncudes bls_msg_includes;
  };

  enum ResponseStatus {
    RESPONSE_COMPLETE = 0,
    RESPONSE_PARTIAL = 101,
    BLOCK_NOT_FOUND = 201,
    GO_AWAY = 202,
    INTERNAL_ERROR = 203,
    BAD_REQUEST = 204,
  };

  struct Response {
    std::vector<TipsetBundle> chain;
    ResponseStatus status;
  };

  CBOR_TUPLE(Request, block_cids, depth, options);
  CBOR_TUPLE(Response, chain, status);

  using OnBlockStored = std::function<void(outcome::result<BlockMsg>)>;

  outcome::result<void> storeResponse(
      const std::shared_ptr<storage::ipfs::IpfsDatastore> &ipld,
      std::vector<TipsetBundle> chain,
      bool store_messages,
      const OnBlockStored &callback);

}  // namespace fc::sync::blocksync

#endif  // CPP_FILECOIN_SYNC_BLOCKSYNC_COMMON_HPP
