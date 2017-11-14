var express = require('express');
var app = express();

app.get('/get_ranking', function (req, res){
  res.send('hi');
  console.log('hi');
});

app.post('/submit_score', function (req, res){

});

app.listen(3000, function() {
  console.log('BillyWilly Ranking Server is Opened on port 3000');
});
