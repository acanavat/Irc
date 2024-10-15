/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acanavat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:56:32 by acanavat          #+#    #+#             */
/*   Updated: 2024/09/30 15:56:34 by acanavat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP 
# define IRC_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <cstring>
#include <stdlib.h>
#include <poll.h>
#include <vector>
#include <map>
#define BUFFER_SIZE 1024

class Client
{
	public :
	Client();
	~Client();
	void setFd(int new_fd);
	int getFd() const;
	std::string waitingRoom;
	bool operator==(const Client &first);
	bool operator!=(const Client &first);
	std::string getNickname() const;
	void setNickname(std::string newNickname);
	void sendMsg(std::string msg);
	private :
	std::string username;
	std::string nickname;
	int fd;
};

class Channel
{
	public :
	Channel();
	~Channel();
	
	std::vector<Client *> getClientlist();
	void addClientlist(Client *newClient);
	void removeClientlist(Client adiosClient);
	int nbrClientlist();

	std::vector<Client *> getClientoperator();
	void addClientoperator(Client *newClient);
	void removeClientoperator(Client adiosClient);
	
	std::vector<Client *> getClientcreator();
	void addClientcreator(Client *newClient);
	void removeClientcreator(Client adiosClient);
	
	void setShortname(std::string channel_name);
	std::string getShortname();
	void msgChannel(Client sender, std::string msg);
	void leaveChannel(Client leave);

	bool getCmdl();
	void setCmdl(bool cmd);
	int getLimitl();
	void setLimitl(int newlimit);
	
	std::string getCmdk();
	void setCmdk(std::string cmd);
	void deleteMdp();
	bool channelMdp();

	std::string getTopic();
	void setTopic(std::string new_topic, Client topic_client);
	void printclientList();

	void setTopicswitch(bool new_switch);
	bool getTopicswitch();

	void setCmdi(Client client,bool cmd);
	bool getCmdi();
	private :
	std::vector<Client *> clientList;
	std::vector<Client *> clientOperator;
	std::vector<Client *> clientCreator;
	std::vector<Client *> clientInvitation;
	std::string shortName;
	bool cmdL;
	int limitL;
	std::string cmdK;
	bool mdp;
	std::string topic;
	bool topic_switch;
	bool cmdi;
};

void rattrapeReddy(std::string msg, Client *client);
#endif 