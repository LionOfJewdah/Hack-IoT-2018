
var fs = require('fs');
var schedule = require('node-schedule');

//set up Web3
var Web3 = require('web3');
Web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:8545"));

//read code
code = fs.readFileSync('contracts/mediator.sol').toString();
solc = require('solc');
compiled = solc.compile(code);
	
//get abi and create contract instance
abi = JSON.parse(compiled.contracts[':mediator'].interface);
var Mediator = new Web3.eth.Contract(abi,'0x0686e360a3ce81d306c20adee3f7c15bf2fe31a3');
byteCode = compiled.contracts[':mediator'].byteCode;

Web3.eth.defaultAccount = Web3.eth.accounts[0];

Mediator.methods.createEstablishment('hackiot:0000',50).send({from: '0xa0045b2a425a2c445fab7f80f57a2abf9d106699'},100000).then(console.log);
Mediator.methods.getBalance('hackiot:0000').call().then(console.log());

var rule = new schedule.RecurrenceRule();
rule.second = [0,20,40];	


schedule.scheduleJob(rule,function(){
	fs.readFile('/../nodes.txt',function(err,data){
		console.log(data);
	});
});