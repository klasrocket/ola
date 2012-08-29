/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * SLPServer.h
 * Copyright (C) 2012 Simon Newton
 */

#ifndef TOOLS_SLP_SLPSERVER_H_
#define TOOLS_SLP_SLPSERVER_H_

#include <termios.h>

#include <ola/ExportMap.h>
#include <ola/io/SelectServer.h>
#include <ola/network/IPV4Address.h>
#include <ola/network/NetworkUtils.h>
#include <ola/network/Socket.h>
#include <ola/network/TCPSocketFactory.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "tools/slp/Base.h"

using ola::network::IPV4Address;
using ola::network::TCPSocket;
using std::string;
using std::vector;

namespace ola {

namespace http {
  class OlaHTTPServer;
}

namespace slp {

/**
 * An SLP Server.
 * TODO(simon): split this into a daemon and a server component.
 */
class SLPServer {
  public:
    struct SLPServerOptions {
      // IP to multicast on
      IPV4Address ip_address;  // The interface IP to multicast on
      bool enable_da;  // enable the DA mode
      bool enable_http;  // enable the HTTP server
      uint16_t http_port;  // port to run the HTTP server on
      uint16_t rpc_port;  // port to run the RPC server on

      SLPServerOptions()
          : enable_da(true),
            enable_http(true),
            http_port(DEFAULT_SLP_HTTP_PORT),
            rpc_port(OLA_SLP_DEFAULT_PORT) {
      }
    };

    SLPServer(ola::network::UDPSocket *udp_socket,
              ola::network::TCPAcceptingSocket *tcp_socket,
              const SLPServerOptions &options,
              ola::ExportMap *export_map);
    ~SLPServer();

    bool Init();
    void Run();
    void Stop();

    static const uint16_t DEFAULT_SLP_PORT;

  private:
    class NodeEntry {
      string service;
    };

    typedef vector<NodeEntry*> NodeList;

    const IPV4Address m_iface_address;
    const uint16_t m_rpc_port;
    ola::io::SelectServer m_ss;
    ola::network::TCPSocketFactory m_tcp_socket_factory;
    ola::network::TCPAcceptingSocket m_tcp_accept_socket;
    ola::network::IPV4Address m_multicast_address;

    // the UDP and TCP sockets for SLP traffic
    ola::network::UDPSocket *m_udp_socket;
    ola::network::TCPAcceptingSocket *m_tcp_socket;

    // The ExportMap & HTTPServer
    ola::ExportMap *m_export_map;
    std::auto_ptr<ola::http::OlaHTTPServer> m_http_server;

    // Used to handle events from the command line for now, remove this later
    ola::io::UnmanagedFileDescriptor m_stdin_descriptor;
    termios m_old_tc;

    // the nodes
    NodeList m_nodes;

    // tcp methods
    void NewTCPConnection(TCPSocket *socket);
    void TCPSocketClosed(TCPSocket *socket);
    void UDPData();

    /*
    void ReceiveTCPData(TCPSocket *socket);
    void SocketClosed(TCPSocket *socket);
    void SendJoinUpdateToClients(const IPV4Address &address, const UID &uid);
    void SendPartUpdateToClients(const IPV4Address &address, const UID &uid);
    void SendStringToClients(const string &output);
    void SendState(TCPSocket *socket);
    */

    // stdin
    void Input();

    // housekeeping methods
    /*
    bool SendPeriodicAdvert();
    bool LookForStaleEntries();
    */

    static const uint16_t DEFAULT_SLP_HTTP_PORT;
    static const char K_SLP_PORT_VAR[];
};
}  // slp
}  // ola
#endif  // TOOLS_SLP_SLPSERVER_H_
