/******************************************************************************
  2016 SVPA Flood Sensor Prototype HTML
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/

var socBaseURL = "https://data.sparkfun.com/output/9JN3JagnELSWrOQvXVN0?grep[source]=";
var rangeBaseURL = "https://data.sparkfun.com/output/n1EQXJJb6mUzDLV52dN5?grep[source]=";
<<<<<<< HEAD
var rangeURL = "";
=======
var plotDataURL = "";
>>>>>>> 2ab8449aa2e6cfb05eb53ccc004f995dc50ae0eb

var graphWidth = 960;
var graphHeight = 500;
var margin = {top: 20, right: 20, bottom: 75, left: 50},
      width = graphWidth - margin.left - margin.right,
      height = graphHeight - margin.top - margin.bottom;

var graphTimeOffset = -500; //hours
var currentTZ = "local";

<<<<<<< HEAD
var graphDeviceID = "380054000c51343334363138";
=======
var graph = "380054000c51343334363138";
>>>>>>> 2ab8449aa2e6cfb05eb53ccc004f995dc50ae0eb

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
//var parseDate = d3.timeParse("%Y-%m-%dT%H:%M:%S.%LZ");
var parseDate = d3.utcParse("%Y-%m-%dT%H:%M:%S.%LZ");

var todayDate = new Date();
var graphStartDate = d3.timeHour.offset(todayDate, graphTimeOffset);
var graphEndDate = d3.timeHour.offset(todayDate, (-1 * graphTimeOffset / 50));

var lastUpdateSpan = d3.select("#lastUpdate").html(todayDate);

console.log( "graphStartDate: " + graphStartDate.getTime());
console.log( "todayDate: " + todayDate.getTime());

var timeScale = d3.scaleTime()
                  .domain([graphStartDate.getTime(),graphEndDate.getTime()])
                  .rangeRound([0,width]);

var timeAxis = d3.axisBottom()
              .scale(timeScale)
              .ticks(d3.timeHour.every(12))
              .tickSizeInner(-height)

              .tickFormat(d3.timeFormat("%b %d, %H:%M"))
              ;

var socLine = d3.line()
            .x(function(d) { return timeScale(d.timestamp.getTime()); })
            .y(function(d) { return socScale(+d.soc); });

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
        //.call(d3.axisBottom(timeScale))
        .call(timeAxis)
        .selectAll("text")
          .attr("y", 0)
          .attr("x", -75)
          .attr("dy", ".35em")
          .attr("transform", "rotate(-90)")
          .style("text-anchor", "start");
        ;


var yAxisArea = plotArea.append("g")
        .attr("class", "yAxisArea")
        .call(d3.axisLeft(socScale))
        .append("text")
         .attr("transform", "rotate(-90)")
         .attr("y", 6)
         .attr("dy", ".71em")
         .style("text-anchor", "end")
         .text("State of Charge (%)");



<<<<<<< HEAD
  console.log("socURL @ SOC CSV call");
  console.log(socURL);

  console.log( "graphStartDate.getUTCDay(): " + graphStartDate.getUTCDate());
=======
console.log( "graphStartDate.getUTCDay(): " + graphStartDate.getUTCDate());
>>>>>>> 2ab8449aa2e6cfb05eb53ccc004f995dc50ae0eb

  var timeFilter = "&gte[timestamp]="
                + (graphStartDate.getUTCMonth()+1) + "-"
                + (graphStartDate.getUTCDate()+1) + "-"
                + graphStartDate.getUTCFullYear()
  console.log( "timeFilter: " + timeFilter);

<<<<<<< HEAD
d3.csv(socBaseURL + graphDeviceID + timeFilter,
      function(d) {
          d.timestamp = parseDate(d.timestamp);
          d.soc = +d.soc;
          return d;
        },
      function(error, socData) {
        if (error) throw error;
        // **** Build State of Charge graph ****

/*        for (socDataPoint of socData) {
          console.log("socDataPoint");
          console.log(socDataPoint.timestamp);

          socDataPoint.timestamp = d3.timeParse(socDataPoint.timestamp);
          socDataPoint.soc = +socDataPoint.soc;
        }
*/
        console.log("socData after timeParse");
        console.log(socData);

        var socPath = socLine(socData);
        //console.log("socPath");
        //console.log(socPath);


        plotArea.append("path")
          .datum(socData)
          .attr("class", "line")
          .attr("d", socPath);

      })
      .on("progress", function(p){
        //update progress bar
        console.log("Loading:" + p);
      })
      ;
//    }
//});
=======
plotDataURL = socBaseURL + graph + timeFilter


function plotSensorData(url, lineColor){

  d3.csv(url,
        function(d) {
            d.timestamp = parseDate(d.timestamp);
            d.soc = +d.soc;
            return d;
          },
        function(error, socData) {
          if (error) throw error;
          // **** Build State of Charge graph ****


          console.log("socData after timeParse");
          console.log(socData);

          var socPath = socLine(socData);
          //console.log("socPath");
          //console.log(socPath);


          plotArea.append("path")
            .datum(socData)
            .attr("class", "line")
            .style("stroke", lineColor)
            .attr("d", socPath);

        })
        .on("progress", function(p){
          //update progress bar
          console.log("Loading:" + p);
        })

        ;
  }

plotSensorData(plotDataURL, "#4682B4"); 
>>>>>>> 2ab8449aa2e6cfb05eb53ccc004f995dc50ae0eb

graph = "210022000c51343334363138";
plotDataURL = socBaseURL + graph + timeFilter
plotSensorData(plotDataURL, "#B22222");

console.log("done with FloodSensorStatus.js");
