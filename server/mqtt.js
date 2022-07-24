// LIB IMPORT
const mqtt = require("mqtt");
const admin = require("firebase-admin");

const base64ToJSON = (s) =>
  s ? JSON.parse(Buffer.from(s, "base64").toString()) : undefined;

admin.initializeApp({
  credential: admin.credential.cert(
    base64ToJSON("ewogICJ0eXBlIjogInNlcnZpY2VfYWNjb3VudCIsCiAgInByb2plY3RfaWQiOiAicHAxMjEyLTZlZDkzIiwKICAicHJpdmF0ZV9rZXlfaWQiOiAiNzIzYjkyOGFjYTk4YWI3Y2U3Yzg5N2VlODRjY2E1N2UwOTc5ZjZjNyIsCiAgInByaXZhdGVfa2V5IjogIi0tLS0tQkVHSU4gUFJJVkFURSBLRVktLS0tLVxuTUlJRXZnSUJBREFOQmdrcWhraUc5dzBCQVFFRkFBU0NCS2d3Z2dTa0FnRUFBb0lCQVFDbEh1QWJRUm5WVlZod1xueFRvaG0va1RCWndscEFualNBc3M5ODFvbUtHK204SGxjaGJOT2dmQnVaWFlxUlkydHVBbmY2azg0OEwxVmZUWlxuKzE1cUVrcGtVeHhRbkZtRERRcFRhWnloNGYyR2RNYitodkNtc3BmMnB4Uk9jVVl0ZTBMMFlqUFgvaEtJOHRURFxuSEw1V3FENnBOS3EzVU1RZmFWYzc2c1ZZQXF5SmFCNHRIQ1ZBc1FUL09pOC83d0JYL3lFbE9veVZIVnlqMk5xYVxuYWtyQkxSS1NROXJtWThHdnVDSDg1czU2NzRtdXJ3ZHRYT29rb3d2WWFhRTNCT1M0SmQ4cEhoMDd5c0g0YlpYYlxuWnpWUUlPTWZBdEd1aHo3YW5qUHVwYy9LTnBCTEtLVkFFcElYanYxZ3pMMER6UEJpMlk0TTJ4THRZdXJmT0RqQlxueVJreURCTlRBZ01CQUFFQ2dnRUFCbFV3VUN0SGc5eVJsMFdoTEovUVJpLzlaSjR0ZU9lTzhpc1EyRFdlWW5LVFxuMWFweUFHMzgxU3U2ZVFSSEkxTVJOc0taS1lvc3RIOFc5SE5GRkRsZVhBbXArTmQrUVYzekt1N09nSkRyeWNmWlxueC9KQjhZckE5RWRGS3BzQ2o0VCtOT1E5UzhmbFN4WlJIYWlSRGo3R3BIT3VKbmkxQk9WajRKcm85YTZva2w1NVxubnZnb3dhWDd6UEMxRFV6NXVqTjRoWWFBbXJkMWQ5WGVEV0ZhdDNOWi9oNExVUGRwSVYyN1hmWTM2UzVyZ1dRaVxuTGtScUNJRU9hbFIzeEVjVUt5SVJWUEE0TDFmaVdVTUdrbUxjZGRQUW5FNTAxTldwLzBvb3MrdWdidlRreDNhV1xuSDdWLzcwU1ZWM2w2aTVjZ1hyWE9sdFZ0WVdxZElrUWhVYkFVNFhBVkVRS0JnUURUWURUTUthbkx3Q2t4dmxjb1xuQVJlWWM0d3l4OXFLcUIvNUJiVGNWT3VLUzJoN2Z6T296RFpOaXRUSEFISmpCL0ZEUHlpUnQreW1Qd01xL3p5MVxueFJyMVdrOFE5SEE3cmFPWlErZGo0OUZJRUZIaEgrcXlwcFBnbG9ZaWtvTWZYMjlGZzZ1dEg2SFgycGJYOTlSRlxuTnc4bUdDWXhhbjlydUNHalZJQXJiK09Cc1FLQmdRREgrczg4eTVWMlhUYVMzbE1rOXEvWnFrdGo2ZC9hcVZlZ1xuT0Fqak5yb1QzWXBPaTdxYWR2TDJJMkh4aWozdkhJUkR3ZnFxN25YVkNPTVF3Qkt4TWFnRlFYeVdlNWg3RXVMYVxuNDI5dms3THFiK1p5cEEwbzlJVmNjNlArbzErL1R4ZXRud0Z2Nm1EalVOd0NrNFFvQVVBUU1xRXp1RnhKSWl3cVxuenZnYWtnbkNRd0tCZ1FDUjBtRWFKL1AzbUUzUy8xanBSRGVlK0xmVG9JUm4rNnh6QXpqT2Q5YzJ4T2NEcTdGYlxuMWVYd2dNSmh0ekV5bUp3Y3BFbThuWUlvL25lVkJoZy8zZ2JIRk5KUEJCTVpXS1R3YXlRS2lYUXJudmFkdElNWlxuVkVuWU9uKzNBaFJyWkU5RFNaNzZCRVNHd1Vvbkc5WFpHaTdkUFBPOFM1ZzBIQWM0d21iWmtJc0hRUUtCZ1FDU1xuUVBIemFvaml5Z1V4b0h0Nld4Zjh2cmpvVjVFcDRCTi96N3RnUVJRUmwwS2hjenVENEY2TWVSQVdrSjMvRkFIL1xuZEF5Wm1sQisvMExoYXpmQWM2cUlQWDZhcDhXNnJQTlFWbW1YVnBQUk5RbDY2bFBka2djK3VDeEYvTUgzcmlhSlxuQ2ZNSklJYS9lTkdPNk1nTVllMnlhdlRPV0pUVnY3SkJsS0NLL2ljNDlRS0JnSEQyWnlSWSt2dHlGaGhxZnZKeVxuS1pneGEzL3ZMTG5FTGdHb0NhTzRhcElEaWdxdXcyc3NJdzhiWkh2eUR3WWNSeGxJWXlTVUNFQUx2ZEpTSUEvOVxuUW9rU2FLSC9wMktuNFVRZkRDSk5RaWJ5d0EreE8wbjNzenRtbk9RRlVNSHZMaUxqa2dNcDBoN0FuTVN4SkVZclxuT0hHeklpT21YVjllT0N0emR0MnlaZkxDXG4tLS0tLUVORCBQUklWQVRFIEtFWS0tLS0tXG4iLAogICJjbGllbnRfZW1haWwiOiAiZmlyZWJhc2UtYWRtaW5zZGstMmwzNmNAcHAxMjEyLTZlZDkzLmlhbS5nc2VydmljZWFjY291bnQuY29tIiwKICAiY2xpZW50X2lkIjogIjExNDY5MjYyNzkwMjA4MzU3OTYzMiIsCiAgImF1dGhfdXJpIjogImh0dHBzOi8vYWNjb3VudHMuZ29vZ2xlLmNvbS9vL29hdXRoMi9hdXRoIiwKICAidG9rZW5fdXJpIjogImh0dHBzOi8vb2F1dGgyLmdvb2dsZWFwaXMuY29tL3Rva2VuIiwKICAiYXV0aF9wcm92aWRlcl94NTA5X2NlcnRfdXJsIjogImh0dHBzOi8vd3d3Lmdvb2dsZWFwaXMuY29tL29hdXRoMi92MS9jZXJ0cyIsCiAgImNsaWVudF94NTA5X2NlcnRfdXJsIjogImh0dHBzOi8vd3d3Lmdvb2dsZWFwaXMuY29tL3JvYm90L3YxL21ldGFkYXRhL3g1MDkvZmlyZWJhc2UtYWRtaW5zZGstMmwzNmMlNDBwcDEyMTItNmVkOTMuaWFtLmdzZXJ2aWNlYWNjb3VudC5jb20iCn0K")
  ),
});

const cloudFirestore = admin.firestore();

cloudFirestore.settings({ timestampsInSnapshots: true });

const client = mqtt.connect("mqtt://ggaomyqh:3wjA27NFU3ET@m16.cloudmqtt.com:16319/");

client.on("connect", function () {
  // system topic event
  client.subscribe('/sensors', function (err) {
    if (!err) {
      console.log('subscribed to /sensors');
    }
  });
});

client.on("reconnect", () => {
  console.log("MQTT client is reconnecting...");
});

client.on("disconnect", () => {
  console.log("MQTT client is disconnecting...");
});

client.on("error", function () {
  console.log("Can't connect");
  client.reconnect();
});

client.on("message", function (topic, message) {

  if (isJsonString(message)) {
    var action = JSON.parse(message);
    console.debug(action);
    switch (topic) {
      case '/sensors':
        // if (isSensorExist(message)) {
        //   registerNewSensor(message);
        // } else {
          updateValue(message)
        // }
        break;
      default:
        break;
    }
  }
});

function isJsonString(str) {
  try {
    JSON.parse(str);
  } catch (e) {
    return false;
  }
  return true;
}

async function updateValue(message) {
    var device = JSON.parse(message);
    let now = new Date();
    const db = cloudFirestore.collection("sensors");
    const doc = db.doc(device.id.toString());

    await doc
    .update({
      pH: device.pH,
      temperature: device.temp,
      updated_at: now.toLocaleString(),
      timestamp: now.valueOf(),
    })
    .catch((e) => {
      console.log(e);
    });
}

async function registerNewSensor(message) {
  var device = JSON.parse(message);
  let now = new Date();
  const db = cloudFirestore.collection("sensors");
  const doc = db.doc(device.id.toString());

  await doc
    .set({
      pH: device.pH,
      temperature: device.temp,
      created_at: now.toLocaleString(),
      timestamp: now.valueOf(),
    })
    .catch((e) => {
      console.log(e);
    });
}


async function isSensorExist(message) {
  var device = JSON.parse(message);
  let sensorRef = cloudFirestore.collection("sensors");
  let not_exist = false;
  console.log(device.id.toString());
  await sensorRef
    .doc(device.id.toString())
    .get()
    .then(function (doc) {
      console.log(doc.exists)
      if (doc.exists) {
        not_exist = false;
      } else {
        not_exist = true;
      }
    })
    .catch(function (error) {
      console.log("Error getting document:", error);
    });

  return not_exist;
}


