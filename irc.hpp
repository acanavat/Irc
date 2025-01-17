/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acanavat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:56:32 by acanavat          #+#    #+#             */
/*   Updated: 2025/01/17 15:53:18 by rbulanad         ###   ########.fr       */
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
#include <algorithm>
#include <iostream>
#include <csignal>
#include <cstdlib>
#define BUFFER_SIZE 1024

std::string assword;

int ft_stoi (std::string &str)
{
	int i;

	std::istringstream(str) >> i;
	return (i);
}

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

	void	setNickname(std::string newnick);
	std::string getNickname() const;
	std::string getUsername() const;
	std::string getId() const;
	
	void	sendMsg(std::string msg, int private_msg);
	void	funcPass(std::vector<std::string> vec);
	void	funcNick(std::string nick);
	void	funcUser(std::vector<std::string> vec);
	void	tryLogin();
	void	boolSetter(int i, bool caca); //0=pass, 1=nick, 2=user 3=first co
	void	stringSetter(int i, std::string neww); //0=nick, 1=user, 2=mode1, 3=mode2, 4=realname, 5=id
	int		isFirstCo();
	private :
	int fd;
	std::string nickname;
	std::string _username;
	std::string _mode1;
	std::string _mode2;
	std::string _realname;
	std::string _id;
	bool _passBool;
	bool _nickBool;
	bool _userBool;
	bool _firstCoBool;
};

class Channel
{
	public :
	Channel();
	~Channel();
	
	std::vector<Client *> &getClientlist();
	void addClientlist(Client *newClient);
	void removeClientlist(Client *adiosClient);
	int nbrClientlist();

	std::vector<Client *> getClientoperator();
	void addClientoperator(Client *newClient);
	void removeClientoperator(Client *adiosClient);
	
	std::vector<Client *> getClientcreator();
	void addClientcreator(Client *newClient);
	void removeClientcreator(Client *adiosClient);
	
	void setShortname(std::string channel_name);
	std::string getShortname();
	void msgChannel(int fdSender, std::string msg, int ignore);
	void leaveChannel(Client leave);

	bool getCmdl();
	void setCmdl(bool cmd);
	int getLimit();
	void setLimit(unsigned int newlimit);
	
	std::string getMdp();
	void setMdp(std::string newMdp);
	void deleteMdp();

	std::string getTopic();
	void setTopic(std::string new_topic, Client topic_client);

	void setTopicswitch(bool new_switch);
	bool getTopicswitch();

	void setCmdi(Client client,bool cmd);
	bool getCmdi();

	void printclientList();
	Client	*findClient(std::string name);
	Client	*isOp(int fd);
	void	addMode(char mode);
	void	removeMode(char mode);
	bool	checkMode(char mode);
	std::vector<char>	getModes();
	std::string	getModeString();
	std::string	toString(int i);


	private :
	std::vector<Client *> clientList;
	std::vector<Client *> clientOperator;
	std::vector<Client *> clientCreator;
	std::vector<Client *> clientInvitation;
	std::vector<char>	modes;
	std::string shortName;
	bool cmdL;
	unsigned int limit;
	std::string _mdp;
	bool isMdp;
	std::string topic;
	bool topic_switch;
	bool cmdi;
};

class	Acommand; //cringe (defined here pcq class Server needs Acommand, but Acommand needs class Server)

class	Server
{
	private:
	std::list<Acommand*> _cmd;
	std::map<std::string, Channel*> chanMap;
    std::map<int, Client*> clientMap;
	public:
	Server();
	~Server();
	
	std::map<std::string, Channel*>	&getChannelMap();
    std::map<int, Client*> &getClientMap();
	void	rattrapeReddy(std::string msg, Client *client);
	void	CmdParser(std::string cmd, Client *client);
	void	Parser(std::string cmd, Client *client);
	void	whichCmd(std::vector<std::string> vec, Client *client);
	Channel	*findChannel(std::string name);
	Client	*findClient(std::string name);
	void	printClients();
	template <class T> static std::string	toString(const T &value)
	{
		std::ostringstream oss;

		oss << value;
		return (oss.str());
	};
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

class	FuncJoin : public Acommand
{
	private:
	public:
	FuncJoin();
	~FuncJoin();

	void	exec(Server *serv, Client *client, std::vector<std::string> vec) const;
	std::string	stringOfUsers(Channel *chan) const;
};

class	FuncPrivMsg : public Acommand
{
	private:
	public:
	FuncPrivMsg();
	~FuncPrivMsg();

	void	exec(Server *serv, Client *client, std::vector<std::string> vec) const;
};

class	FuncQuit : public Acommand
{
	private:
	public:
	FuncQuit();
	~FuncQuit();

	void	exec(Server *serv, Client *client, std::vector<std::string> vec) const;
};

class	FuncMode : public Acommand
{
	private:
	public:
	FuncMode();
	~FuncMode();

	bool	addMode(Channel *chan, std::vector<std::string> vec, Client *client) const;
	bool	removeMode(Channel *chan, std::vector<std::string> vec, Client *client) const;
	void	exec(Server *serv, Client *client, std::vector<std::string> vec) const;
	//addMode and removeMode functions (different than those in Channel
};
#endif
