/**
 * The mediator contract does this and that...
 */

pragma solidity ^0.4.19;


contract mediator {
	struct Event {
		address addr;
		uint people;
		string identifier;
		uint price;
		uint lastUpdated;
		uint balance; //balance in wei
	}

	Event[] public events;
	mapping (address => bool) exist;
	mapping (string => Event) identifierToEvent;

	modifier onlyBy(address _account) { 
		require (msg.sender == _account); 
		_; 
	}	

	//Register a new bar,club,event etc.
	function createEstablishment (string identifier,uint price) public {
		require(!exist[msg.sender]);	
		events.push(Event(msg.sender,0,identifier,price,now,0));

		exist[msg.sender] = true;
		identifierToEvent[identifier] = events[events.length-1];
	}
	
	//Send population data from sensor
	function setPopulation (string identifier,uint people) public {
		require(msg.sender == identifierToEvent[identifier].addr); //require that the caller be the owner of the establishment, otherwise throw an error
		identifierToEvent[identifier].people = people;
		identifierToEvent[identifier].lastUpdated = now;
	}
	
	//Retrieve population data
	function getPopulation (string identifier) public payable returns(uint256 people, uint lastUpdated) {
		require(msg.value > identifierToEvent[identifier].price); //require that the sender sends more than the what the establishment set as the price of data
		return(identifierToEvent[identifier].people,identifierToEvent[identifier].lastUpdated);	
	}

	//Get sensor balance
	function getBalance (string identifier) public returns(uint balance){
		require(msg.sender == identifierToEvent[identifier].addr); //require that the caller be the owner of the establishment, otherwise throw an error
		return(identifierToEvent[identifier].balance);
	}

	//Withdraw sensor balance to sender address if sender address is the creator of the event
	function withdraw(uint amount,string identifier) public {
		require(amount < identifierToEvent[identifier].balance);
		require(msg.sender == identifierToEvent[identifier].addr);
		msg.sender.send(amount);
	}
	
	

}
