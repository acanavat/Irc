/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acanavat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:56:32 by acanavat          #+#    #+#             */
/*   Updated: 2024/10/22 15:01:56 by rbulanad         ###   ########.fr       */
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
#include <sstream>
#include <map>
#include <list>
#define BUFFER_SIZE 1024

std::string assword;

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

	std::string getUsername() const;
	
	void sendMsg(std::string msg, int private_msg);
	void funcPass(std::vector<std::string> vec);
	void funcNick(std::string nick);
	void funcUser(std::vector<std::string> vec);
	void tryLogin();
	void boolSetter(int i, bool caca); //0=pass, 1=nick, 2=user
	void stringSetter(int i, std::string neww); //0=nick, 1=user, 2=mode1, 3=mode2, 4=realname
	private :
	int fd;
	std::string nickname;
	std::string _username;
	std::string _mode1;
	std::string _mode2;
	std::string _realname;
	bool _passBool;
	bool _nickBool;
	bool _userBool;
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

class	Acommand; //cringe (defined here pcq class Server needs Acommand, but Acommand needs class Server)

class	Server
{
	private:
	std::list<Acommand*> _cmd;
	std::map<std::string, Channel*> chanList;
	public:
	Server();
	~Server();

	void	rattrapeReddy(std::string msg, Client *client);
	void	CmdParser(std::string cmd, Client *client);
	void	Parser(std::string cmd, Client *client);
	void	whichCmd(std::vector<std::string> vec, Client *client);
	int		findChannel(std::string name, int clientFd);
};

class	Acommand
{
	private:
	std::string _name;
	public:
	Acommand(std::string name);
	virtual ~Acommand();

	std::string	getName();
	virtual void		exec(Server *serv, Client *client, std::vector<std::string> vec) const = 0;
};

class	FuncCap : public Acommand
{
	private:
	public:
	FuncCap();
	~FuncCap();

	void	exec(Server *serv, Client *client, std::vector<std::string> vec) const;
};

class	FuncPass : public Acommand
{
	private:
	public:
	FuncPass();
	~FuncPass();

	void	exec(Server *serv, Client *client, std::vector<std::string> vec) const;

};

class	FuncNick : public Acommand
{
	private:
	public:
	FuncNick();
	~FuncNick();

	void	exec(Server *serv, Client *client, std::vector<std::string> vec) const;
};

class	FuncUser : public Acommand
{
	private:
	public:
	FuncUser();
	~FuncUser();

	void	exec(Server *serv, Client *client, std::vector<std::string> vec) const;
};

class	FuncPing : public Acommand
{
	private:
	public:
	FuncPing();
	~FuncPing();

	void	exec(Server *serv, Client *client, std::vector<std::string> vec) const;
};

class	FuncPrivMsg : public Acommand
{
	private:
	public:
	FuncPrivMsg();
	~FuncPrivMsg();

	void	exec(Server *serv, Client *client, std::vector<std::string> vec) const;
};
#endif
