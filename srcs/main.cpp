/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: estarck <estarck@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 12:22:31 by estarck           #+#    #+#             */
/*   Updated: 2023/03/16 15:58:06 by estarck          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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

bool	checkExtension(std::string extension)
{
	std::string tmp = extension.substr(extension.size() - 5, extension.size());
	if(tmp != ".conf")
		return (false);
	return (true);
}

int main(int argc, char ** argv)
{
	//On verifie la presence du fichier .conf
	if (argc < 2)
	{
		std::cerr << "\033[1;31mError : Need < nginx_config_file >\033[0m" << std::endl;
		exit (-1);
	}
	
	//On verifie l'extension du fichier.
	std::string	extension(argv[1]);
	if (!checkExtension(extension))
	{
		std::cerr << "\033[1;31mError : Need extension .conf\033[0m" << std::endl;
		exit (-1);
	}

	//Ouverture du fichier
	const std::string  config_file_path(argv[1]);
	std::ifstream config_file(config_file_path.c_str());
	if (!config_file.is_open())
		std::cerr << "\033[1;31mError : Opening " << config_file_path << "\003[0m" << std::endl;

	//On compte le nombre de serveur qu'on enregistre ensuite dans my_config.
	unsigned int nbr_server = countServer(config_file);

	//Creation du server
	Master	my_webServe(config_file, nbr_server);

    return 0;
}
