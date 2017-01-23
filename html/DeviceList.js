/******************************************************************************
  2016 SVPA Flood Sensor Prototype HTML
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/

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
            .attr("id", (function(d) {
                return (d.DeviceName+"-ID")
              })
            )
            .attr("class", ("DeviceID"));

  devicePara.append("span").text(", ");
  devicePara.append("span")
            .text(function(d) {return ("v"+d.FirmwareVersion)})
            .attr("id", (function(d) {
                return (d.DeviceName+"-FirmwareVersion")
              })
            )
            .attr("class", ("DeviceID"));

  devicePara.append("span").text(", ");
  devicePara.append("a")
            .text("Readings ")
            .attr("id", (function(d) {
                return (d.DeviceName+"-ReadingURL")
              })
            )
            .attr("href", function(d) {
               var readingURL = readingBaseURL + d.DeviceID;
               d.readingURL = readingURL;
              return readingURL;
                      });

  devicePara.append("span").text(", ");
  devicePara.append("a")
            .text("soc ")
            .attr("id", (function(d) {
                return (d.DeviceName+"-socURL")
              })
            )
            .attr("href", function(d) {
               var socURL = socBaseURL + d.DeviceID;
               d.socURL = socURL;
              return socURL;
            });

  devicePara.append("span").text(", ");
  devicePara.append("a")
            .text("Range ")
            .attr("id", (function(d) {
                return (d.DeviceName+"-rangeURL")
              })
            )
            .attr("href", function(d) {
               var rangeURL = rangeBaseURL + d.DeviceID;
               d.rangeURL = rangeURL;
              return rangeURL;
            });

  devicePara.append("span").text(", ");
  devicePara.append("span")
            .text(function(d) {
              console.log(d.lastSeen);
               if (typeof d.lastSeen !== "undefined") {
                 d.lastSeen = "--"
               }
              return d.lastSeen;
            })
            .attr("id", (function(d) {
                return (d.DeviceName+"-lastSeen")
              })
            );
});
console.log("done with FloodSensorStatus.js");
