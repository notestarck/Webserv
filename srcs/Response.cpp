/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estarck <estarck@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 15:50:59 by estarck           #+#    #+#             */
/*   Updated: 2023/04/17 19:05:44 by estarck          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"

void createHttpResponse(Client &client, int statusCode, const std::string &contentType)
{
    std::string response;
    std::string statusMessage;

    switch (statusCode)
    {
        case 200:
            statusMessage = "OK";
            break;
        case 201:
            statusMessage = "Created";
            break;
        case 204:
            statusMessage = "No Content";
            break;
        default:
            statusMessage = "Unknown Status";
    }

    response.append("HTTP/1.1 " + std::to_string(statusCode) + " " + statusMessage + "\r\n");
    response.append("Content-Type: " + contentType + "\r\n");
    response.append("Content-Length: " + std::to_string(client._bodyRep.size()) + "\r\n");
    response.append("Accept-Charset: utf-8\r\n");
    response.append("Connection: Closed\r\n");
    response.append("\r\n");
    response.append(client._bodyRep);

    client._sizeRep = response.size();

    //On enregistre l'ensemble de la reponse dans le client pour l'envoyer en plusieurs fois.
    client._response = response;
}

void sendHttpResponse(Client &client)
{
    int optval = 0;
    socklen_t  optlen = sizeof(optval);
    if(getsockopt(client._csock, SOL_SOCKET, SO_SNDBUF, &optval, &optlen) == -1 || optval <= 0)
    {
        std::cerr << "Error : 500 sending data to client getsockopt(): " << client._csock << std::endl;
        sendErrorResponse(client, 500);
        client._keepAlive = false;
        return;
    }
    
    ssize_t remainingSize = client._sizeRep - client._sizeSend;
    
    const char* bodyData = client._response.data() + client._sizeSend;
    ssize_t bodySize = std::min(remainingSize, static_cast<ssize_t>(optval));
    std::string response(bodyData, bodySize);

    ssize_t sentBytes = send(client._csock, response.data(), response.size(), 0);
    client._sizeSend += sentBytes;
    std::cout << "optval : " << optval << " _sizeSend : " << client._sizeSend << " bodySize : " << bodySize << " sentByte : " << sentBytes << "_sizeBodyRep : " << client._sizeRep << std::endl;
    //sleep(1);
    if (sentBytes == -1)
    {
        perror("Erreur lors de l'envoi de la réponse");
        //client._keepAlive = false;
    }
}

void sendHttpResponse(Client &client, int statusCode, const std::string &contentType, std::ifstream &body)
{
    std::string response;
    std::string statusMessage;

    switch (statusCode)
    {
        case 200:
            statusMessage = "OK";
            break;
        case 201:
            statusMessage = "Created";
            break;
        case 204:
            statusMessage = "No Content";
            break;
        default:
            statusMessage = "Unknown Status";
    }

    response.append("HTTP/1.1 " + std::to_string(statusCode) + " " + statusMessage + "\r\n");
    response.append("Content-Type: " + contentType + "\r\n");
    body.seekg(0, std::ios::end);
    response.append("Content-Length: " + std::to_string(body.tellg()) + "\r\n");
    body.seekg(0, std::ios::beg);
    response.append("Accept-Charset: utf-8\r\n");
    response.append("Connection: Closed\r\n");
    response.append("\r\n");
    std::string line;
    while (getline(body, line))
    {
        response.append(line + "\n");
        line.clear();
    }
    
    if (send(client._csock, response.c_str(), response.length(), 0) == -1)
        perror("Erreur lors de l'envoi de la réponse");
}

void sendHttpResponse(Client &client, int statusCode, const std::string &contentType, const std::string &body)
{
    std::string response;
    std::string statusMessage;

    switch (statusCode)
    {
        case 200:
            statusMessage = "OK";
            break;
        case 201:
            statusMessage = "Created";
            break;
        case 204:
            statusMessage = "No Content";
            break;
        default:
            statusMessage = "Unknown Status";
    }

    response.append("HTTP/1.1 " + std::to_string(statusCode) + " " + statusMessage + "\r\n");
    response.append("Content-Type: " + contentType + "\r\n");
    response.append("Content-Length: " + std::to_string(body.length()) + "\r\n");
    response.append("Accept-Charset: utf-8\r\n");
    response.append("Connection: Closed\r\n");
    response.append("\r\n");
    response.append(body);
    
    std::cout << response.c_str() << std::endl;
    if (send(client._csock, response.c_str(), response.length(), 0) == -1)
        perror("Erreur lors de l'envoi de la réponse");
}

void sendErrorResponse(Client &client, int code)
{
    std::string response;
    std::string message;

    switch (code)
    {
        case 400:
            message = "Bad Request";
            break;
        case 403:
            message = "Forbidden";
            break;
        case 404:
            message = "Not Found";
            break;
        case 405:
            message = "Method Not Allowed";
            break;
		case 408:
			message = "Request Timeout";
			break;
		case 413:
			message = "Content Too Large";
			break;
		case 414:
			message = "URI Too Long";
			break;
        case 500:
            message = "Internal Server Error";
            break;
		case 505:
			message = "HTTP Version Not Supported";
			break;
        default:
            message = "Unknown Error";
    }

    response = "HTTP/1.1 " + std::to_string(code) + " " + message + "\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Accept-Charset: utf-8, iso-8859-1;q=0.5\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += "<!DOCTYPE html>\n";
    response += "<html lang=\"en\">\n";
    response += "<head>\n";
    response += "    <meta charset=\"UTF-8\">\n";
    response += "    <title>" + std::to_string(code) + " " + message + "</title>\n";
    response += "</head>\n";
    response += "<body>\n";
    response += "    <h1>Error " + std::to_string(code) + ": " + message + "</h1>\n";
    response += "</body>\n";
    response += "</html>";

    if (send(client._csock, response.c_str(), response.length(), 0) == -1)
        perror("Erreur lors de l'envoi de la réponse d'erreur");
}