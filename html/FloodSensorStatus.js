/******************************************************************************
  2016 SVPA Flood Sensor Prototype HTML
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/

var socBaseURL = "https://data.sparkfun.com/output/9JN3JagnELSWrOQvXVN0?grep[source]=";
var socURL = "";
var rangeBaseURL = "https://data.sparkfun.com/output/n1EQXJJb6mUzDLV52dN5?grep[source]=";
var rangeURL = "";

var deviceArray;

var graphWidth = 960;
var graphHeight = 500;
var margin = {top: 20, right: 20, bottom: 30, left: 50},
      width = graphWidth - margin.left - margin.right,
      height = graphHeight - margin.top - margin.bottom;

var graphTimeOffset = -72; //hours

var parseTime = d3.timeParse("%d-%b-%y");

var x = d3.scaleTime()
    .rangeRound([0, width]);

var y = d3.scaleLinear()
    .rangeRound([height, 0]);

var line = d3.line()
    .x(function(d) { return x(d.timestamp); })
    .y(function(d) { return y(d.soc); });

var socScale = d3.scaleLinear()
                  .domain([0,100])
                  .rangeRound([height, 0]);

var colorScale = d3.scaleLinear()
                  .domain([0,100])
                  .range(["red","green"]);

var socAxis = d3.axisLeft()
                .scale(socScale)
                .ticks(10);

var formatTime = d3.timeFormat("%H:%M");
var parseDate = d3.timeParse("%d-%b-%y");

var todayDate = new Date();
var graphStartDate = d3.timeHour.offset(todayDate, graphTimeOffset);

var lastUpdateSpan = d3.select("#lastUpdate").html(todayDate);

console.log( "graphStartDate: " + graphStartDate.getTime());
console.log( "todayDate: " + todayDate.getTime());

var timeScale = d3.scaleTime()
                  .domain([graphStartDate.getTime(),todayDate.getTime()])
                  .rangeRound([0,width]);

var timeAxis = d3.axisBottom()
              .scale(timeScale)
              .ticks(d3.timeMinute.every(120))
              ;

var socLine = d3.line()
            .x(function(d) { return timeScale(d.timestamp); })
            .y(function(d) { return socScale(d.soc); });

var graphCanvas = d3.select("#graph")
                .append("svg:svg")
                .attr("width",graphWidth)
                .attr("height",graphHeight)
                .attr("border","2px");

var plotArea = graphCanvas.append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")")
        .attr("class", "plotArea")



var xAxisArea = plotArea.append("g")
        .attr("class", "xAxisArea")
        .attr("transform", "translate(0," + height + ")")

        .call(d3.axisBottom(timeScale));


var yAxisArea = plotArea.append("g")
        .attr("class", "yAxisArea")
        .call(d3.axisLeft(socScale))
        .append("text")
         .attr("transform", "rotate(-90)")
         .attr("y", 6)
         .attr("dy", ".71em")
         .style("text-anchor", "end")
         .text("State of Charge (%)");

// **** Build Devices List ****
d3.csv("Devices.csv", function(devices) {

  console.log(devices);
  console.log("sorting...");

  devices.sort(function(a, b) {
    var nameA = a.DeviceName.toUpperCase(); // ignore upper and lowercase
    var nameB = b.DeviceName.toUpperCase(); // ignore upper and lowercase
    if (nameA < nameB) {
      return -1;
    }
    if (nameA > nameB) {
      return 1;
    }

    // names must be equal
    return 0;
  })

  var deviceDiv = d3.select("#deviceList").append("div");

  var deviceList = deviceDiv.selectAll("div")
//  var deviceList = d3.select("#deviceList")
      .data(devices)
      .enter()

  //@TODO Convert from Paragraphs to some kind of table like layout
  var devicePara = deviceList.append("p")
          .attr("id", function(d) {
            return d.DeviceName;
          })
          .append("span")
          .attr("id", function(d) {
            return (d.DeviceName + "-Name");
          })
          .attr("class", "DeviceName")
          .text(function(d) {
            console.log(d);
            return d.DeviceName;
          });

  devicePara.append("span").text(", ");
  devicePara.append("span")
            .text(function(d) {return d.DeviceID})
            .attr("id", (function(d) {return d.DeviceName}+"-ID"))
            .attr("class", ("DeviceID"));

  devicePara.append("span").text(", ");
  devicePara.append("a")
            .text("soc ")
            .attr("href", function(d) {
               socURL = socBaseURL + d.DeviceID;
               d.socURL = socURL;
              return socURL;
            });

  devicePara.append("span").text(", ");
  devicePara.append("a")
            .text("Range ")
            .attr("href", function(d) {
               rangeURL = rangeBaseURL + d.DeviceID;
               d.rangeURL = rangeURL;
              return rangeURL;
            });

  devicePara.append("span").text(", ");
  devicePara.append("span")
            .text("TBD")
            .attr("id", function(d) {
              console.log(d.lastSeen);
               if (typeof d.lastSeen !== "undefined") {
                 d.lastSeen = "TBD"
               }
              return d.lastSeen;
            });

  deviceArray = Object.create(devices);
  console.log("devices @ SOC CSV call");
  console.log(devices);

  console.log("socURL @ SOC CSV call");
  console.log(socURL);

  console.log( "graphStartDate.getUTCDay(): " + graphStartDate.getUTCDate());

  var timeFilter = "&gte[timestamp]="
                + (graphStartDate.getUTCMonth()+1) + "-"
                + (graphStartDate.getUTCDate()+1) + "-"
                + graphStartDate.getUTCFullYear()
console.log( "timeFilter: " + timeFilter);

  d3.csv((socURL+timeFilter), function(socData) {
      // **** Build State of Charge graph ****
      console.log("socData @ SOC CSV call");
      console.log(socData);


      plotArea.append("path")
          .datum(socData)
          .attr("class", "line")
          .attr("d", socLine);



    });

});


console.log("done");
