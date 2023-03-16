/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estarck <estarck@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD
/*   Created: 2023/03/15 12:22:31 by estarck           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/03/15 18:54:13 by estarck          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

=======
/*   Created: 2023/03/13 06:23:52 by estarck           #+#    #+#             */
/*   Updated: 2023/03/14 12:33:16 by estarck          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ParsConfig.hpp"
#include "../include/Server.hpp"
#include "../include/Master.hpp"
>>>>>>> 427a1fd6ceeddf9831520b4a3025cdded7f9f907
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
=======
/*   Updated: 2023/03/16 14:25:03 by estarck          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


>>>>>>> master
#include <iostream>
#include <fstream>
#include <string>

#include "../include/Master.hpp"

int countServer(std::ifstream &config_file)
{
	int     nbr_server = 0;
	char    *buf;

	while (!config_file.eof())
	{
		std::string  line;

		getline(config_file, line);
		if (line == "server {")
			nbr_server++;
	}
	config_file.seekg(0);
	return (nbr_server);
}

int main(int argc, char ** argv)
{
	//On verifie la presence du fichier .conf, il faut encore gerer l'extension du fichier.
	if (argc < 2)
	{
		std::cerr << "\033[1;31mError : Need < nginx_config_file >\033[0m" << std::endl;
		exit (-1);
	}

	//Oouverture du fichier
	const std::string  config_file_path(argv[1]);
	std::ifstream config_file(config_file_path.c_str());
	if (!config_file.is_open())
		std::cerr << "\033[1;31mError : Opening " << config_file_path << "\003[0m" << std::endl;

	//On compte le nombre de serveur qu'on enregistre ensuite dans my_config.
	unsigned int nbr_server = countServer(config_file);

<<<<<<< HEAD
	//Creation des serveurs a parti du fichier de config.
	std::vector<Server *>  my_server;
	for (int i = 0; i < nbr_server; i++)
	{
	    Server *tmp = new Server(*my_config[i]);
	    my_server.push_back(tmp);
	}
<<<<<<< HEAD
=======

	//Acceptation des connexions.
	//Old client : Client	my_client(nbr_server, my_config, my_server);
	Master start_connect(my_server);
>>>>>>> 427a1fd6ceeddf9831520b4a3025cdded7f9f907
	
	//Creation de la connexion
	Connection	my_connect(my_server);
	
		//delete my_server<Server *>
	for (int i = 0; i < nbr_server; i++)
		delete my_server[i];
=======
	//Creation du server
	Master	my_webServe(config_file, nbr_server);
>>>>>>> master

    return 0;
}
