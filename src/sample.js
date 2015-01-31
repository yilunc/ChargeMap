Pebble.addEventListener('ready', function(e) {
  console.log('PebbleKit JS Ready!');
  
  // Construct a dictionary
  var dict = {
    'NAME_KEY':'Station 1'
  };
  
  /*var dict2 = {
    'ADDRESS_KEY': '123 UCSB'
  };
  
  var dict3 = {
    'CITY_KEY': 'Santa Barbara'
  };
  
  var dict4 = {
    'ZIP_KEY': '90000'
  };
  
  var dict5 = {
    'HOURS_KEY': 'Hours'
  };
  
  var dict6 = {
    'CONTYPE_KEYP': 'Type'
  };
  
  var dict7 = {
    'ACCESS_KEY': 'Public'
  };*/
  
  // Send a string to Pebble
  Pebble.sendAppMessage(dict,
    function(e) {
      console.log('Send successful.');
    },
    function(e) {
      console.log('Send failed!');
    }
  );
  /*Pebble.sendAppMessage(dict2,
    function(e) {
      console.log('Send successful.');
    },
    function(e) {
      console.log('Send failed!');
    }
  );
  Pebble.sendAppMessage(dict3,
    function(e) {
      console.log('Send successful.');
    },
    function(e) {
      console.log('Send failed!');
    }
  );
  Pebble.sendAppMessage(dict4,
    function(e) {
      console.log('Send successful.');
    },
    function(e) {
      console.log('Send failed!');
    }
  );
  Pebble.sendAppMessage(dict5,
    function(e) {
      console.log('Send successful.');
    },
    function(e) {
      console.log('Send failed!');
    }
  );
  Pebble.sendAppMessage(dict6,
    function(e) {
      console.log('Send successful.');
    },
    function(e) {
      console.log('Send failed!');
    }
  );
  Pebble.sendAppMessage(dict7,
    function(e) {
      console.log('Send successful.');
    },
    function(e) {
      console.log('Send failed!');
    }
  );*/
});