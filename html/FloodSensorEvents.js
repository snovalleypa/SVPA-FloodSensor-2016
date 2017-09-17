/******************************************************************************
  2017 SVPA Flood Sensor Prototype HTML
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/

var particle = new Particle();
var events;

var todayDate = new Date();

var lastUpdateSpan = d3.select("#lastUpdate").html(todayDate);

//Get your devices events
particle.getEventStream({ product: productID, auth: token }).then(function(stream) {
  stream.on('event', function(data) {
    console.log("Event: " + JSON.stringify(data));
    events.unshift(data);
    d3.select("#lastUpdate").html(new Date());

  });
});


var eventsDiv = d3.select("#events").append("div");

var eventsList = eventsDiv.selectAll("div")
    .data(events)
    .enter()
    .append("div")
      .attr("class", "eventDiv")
      .text(function(d) {
        console.log(d);
        return d.data;
      });

/*
//@TODO Convert from Paragraphs to some kind of table like layout
var eventPara = eventList.append("p")
        .attr("id", function(d) {
          return d.data;
        })
        .append("span")
        .attr("id", function(d) {
          return (d.name + "-Name");
        })
        .attr("class", "DeviceName")
        .text(function(d) {
          console.log(d);
          return d.name;
        });

devicePara.append("span").text(", ");

*/


console.log("done with FloodSensorEvents.js");
