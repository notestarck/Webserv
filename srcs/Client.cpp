/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estarck <estarck@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:08:07 by estarck           #+#    #+#             */
/*   Updated: 2023/04/13 18:28:33 by estarck          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(ParsConfig &config, Server &server, std::vector<ParsConfig::Location> &location) :
	_keepAlive(true),
	_contentLenght(0),
	_sizeBody(0),
	_sizeResp(0),
	_httpVersion(""),
	_uri(""),
	_method(UNKNOWN),
	_body(""),
	_config(config),
	_server(server),
	_location(location)

{
	_requestStr.str(std::string());
	_csock = 0;
}

Client::Client(const Client &srcs) :
    _config(srcs._config),
	_server(srcs._server),
	_location(srcs._location)
{ *this = srcs; }

Client::~Client()
{}

Client &Client::operator=(const Client &srcs)
{
	if (this != &srcs)
	{
		_csock = srcs._csock;
		_csin = srcs._csin;
		_crecSize = srcs._crecSize;
		_lastGetTime = srcs._lastGetTime;
		_keepAlive = srcs._keepAlive;
		_requestStr << srcs._requestStr.str();
		_contentLenght = srcs._contentLenght;
		_sizeBody = srcs._sizeBody;
		_sizeResp = srcs._sizeResp;
		_method = srcs._method;
		_httpVersion = srcs._httpVersion;
		_uri = srcs._uri;
		_headers = srcs._headers;
		_body << srcs._body.str();
		_config = srcs._config;
		_server = srcs._server;
		_location = srcs._location;
	}
	return (*this);
}