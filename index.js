const express = require('express');
const bodyParser = require('body-parser');

const app = express();

const port = 5000;

// Middleware to parse JSON payloads
app.use(bodyParser.json());

// POST endpoint to handle light sensor data
app.post('/api/lightSensor', (req, res) => {
  const lightIntensity = req.body.data.light;

  let numLights = 0;
  if (lightIntensity >= 500 && lightIntensity < 700) {
    numLights = 1;
  } else if (lightIntensity >= 700 && lightIntensity < 900) {
    numLights = 2;
  } else if (lightIntensity >= 900) {
    numLights = 3;
  }

  const response = {
    error: false,
    message: 'Light sensor data received',
    data: {
      numLights: numLights
    }
  };

  res.status(200).json(response);
});

// Start the server
app.listen(port, () => {
  console.log(`Server running on port ${port}`);
});
