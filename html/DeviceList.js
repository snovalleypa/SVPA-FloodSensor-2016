/******************************************************************************
  2017 SVPA Flood Sensor Prototype HTML
  by Tom Sayles <TSayles@Soot-n-Smoke.com>

*******************************************************************************/


var url = 'https://api.particle.io/v1/products/'
  url += productID +'/devices?access_token=' + token;
  console.log("url: " + url);

// **** Build Devices List ****
//d3.json("Devices.json", function(devicesData) {
d3.json(url, function(devicesData) {
  var devices = devicesData.devices;
  console.log(devices);
  console.log("sorting...");



/* Sort by Device Name
  devices.sort(function(a, b) {
    var nameA = a.name.toUpperCase(); // ignore upper and lowercase
    var nameB = b.name.toUpperCase(); // ignore upper and lowercase
    if (nameA < nameB) {
      return -1;
    }
    if (nameA > nameB) {
      return 1;
    }

    // names must be equal
    return 0;
  })
*/
 // Sort by Last Seen
  devices.sort(function(a, b) {

    return d3.descending(a.last_handshake_at, b.last_handshake_at);

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
            return d.name;
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
  devicePara.append("span")
            .text(function(d) {return d.id})
            .attr("id", (function(d) {
                return (d.name+"-ID")
              })
            )
            .attr("class", ("DeviceID"));

  devicePara.append("span").text(", ");
  devicePara.append("span")
            .text(function(d) {return ("v"+d.firmware_version)})
            .attr("id", (function(d) {
                return (d.name+"-FirmwareVersion")
              })
            )
            .attr("class", ("DeviceID"));

  devicePara.append("span").text(", ");
  devicePara.append("span")
            .text(function(d) {
              var timestamp = d3.isoParse(d.last_handshake_at);
              return (timestamp);
            })
            .attr("id", (function(d) {
                return (d.name+"-LastConnection")
              })
            )
            .attr("class", ("LastConnection"));

/*
  devicePara.append("span").text(", ");
  devicePara.append("a")
            .text("Readings ")
            .attr("id", (function(d) {
                return (d.name+"-ReadingURL")
              })
            )
            .attr("href", function(d) {
               var readingURL = readingBaseURL + d.id;
               d.readingURL = readingURL;
              return readingURL;
                      });

  devicePara.append("span").text(", ");
  devicePara.append("a")
            .text("soc ")
            .attr("id", (function(d) {
                return (d.name+"-socURL")
              })
            )
            .attr("href", function(d) {
               var socURL = socBaseURL + d.id;
               d.socURL = socURL;
              return socURL;
            });

  devicePara.append("span").text(", ");
  devicePara.append("a")
            .text("Range ")
            .attr("id", (function(d) {
                return (d.name+"-rangeURL")
              })
            )
            .attr("href", function(d) {
               var rangeURL = rangeBaseURL + d.id;
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
                return (d.id+"-lastSeen")
              })
            );
*/
});
console.log("done with DeviceList.js");
