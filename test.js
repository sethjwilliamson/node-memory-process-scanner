const scan = require('./index');

/*
scan("Legends of Runeterra", "ROUND (1)").then( response => {
  console.log(response);
})*/
console.log("TEST")
scan("Legends of Runeterra", "ROUND (1)").then( roundNumber => {
  console.log(roundNumber)
})
console.log("TEST2")
