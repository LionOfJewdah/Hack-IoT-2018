const http = require('http')
const fs = require('fs')
const port = 3000
//const bootstrap = require('bootstrap')
//const express = require('express')
//var app = express.createServer(express.logger());
//app.use(express.bodyParser());

const original_response = {
  number_of_people: 4,
  vibe: "lit"
};

function errorHandler (err) {
  console.log("fuck", err);
}

const requestHandler = (request, response) => {
  const { method, url } = request;
  console.log(method, url)
  let my_response = Object.assign({}, original_response);
  let body = [];
  request.on('data', (chunk) => {
    body.push(chunk);
  }).on('end', () => {
    body = Buffer.concat(body).toString();
    console.log(body);
    try {
      body = JSON.parse(body);
      my_response.number_of_people = body.number_of_people;
    } catch (err) {
      errorHandler(err);
      console.log("Could not parse JSON");
    }
    console.log(JSON.stringify(my_response));
    response.write(JSON.stringify(my_response));
    response.end();
  }).on('error', errorHandler);
  response.writeHead(200,
    { 'Content-Type': 'application/json' });
  //response.write(JSON.stringify(my_response));
  //response.end('Hello Node.js Server!')
}

const server = http.createServer(requestHandler)

server.listen(port, (err) => {
  if (err) {
    return console.log('something bad happened', err)
  }

  console.log(`server is listening on ${port}`)
})
