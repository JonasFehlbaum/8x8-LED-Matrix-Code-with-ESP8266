const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>LED Matix Display</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="msapplication-TileColor" content="#da532c">
  <meta name="theme-color" content="#ffffff">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }

  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
</head>
<body>
  <div class="topnav">
    <h1>LED Matrix Display</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Controlls</h2>
      <p class="state">state: <span id="state">%STATE%</span></p>
      <p><button id="button" class="button">Toggle</button></p>
      <div class="slidercontainer">
        <label for='brightness'>Set brightness:</label>
        <input type="range" min="1" max="255" value="50" class="slider" id="brightnessInput" ">
        <br><br>
        <label>Brightness:</label>
        <label id = "bright">0</label>
        <br><br>
        <label for='speedy'>Set speed:</label>
        <input type="range" max="1000" min="1" value="100" class="slider" id="speedInput" ">
        <br><br>
        <label>Delay: </label>
        <label id = "speed">0</label>
        <br><br>
      </div>
      <div class="patternContainer"><label for='pattern'>Choose a pattern:</label>
        <select name='pattern' id='patterns'>
          <option id='none' value='none'>None</option>
          <option id='noise' value='noise'>Noise</option>
          <option id='rainbow' value='rainbow'>Rainbow</option>
          <option id='matrix' value='matrix'>Rainball</option>
          <option id='fire' value='fire'>Fire</option>
          <option id='windows' value='windows'>Windows</option>
          <option id='baustrahler' value='baustrahler'>Baustrahler</option>
         </select>
         <br><br>
       </div>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  var trueBrightness;
  var trueSpeed;
  var list = document.getElementById('patterns');
  
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var state;
    console.log("What I heard = " + event.data); 
    var payload = event.data;
    var payloadArray = Array.from(payload);
    
    if (payloadArray[0] == '*'){
      if (payloadArray[1] == '1'){
        state = "ON";
      }
      else {
        state = "OFF";
      }
      document.getElementById('state').innerHTML = state;
    }
    
    if(payloadArray[0] == "#" && payloadArray[1] == "b"){
      trueBrightness = parseInt(payload.substring(2));
      document.getElementById('brightnessInput').value = trueBrightness;
      document.getElementById('bright').innerHTML = trueBrightness;
    }
    
    if(payloadArray[0] == "#" && payloadArray[1] == "s"){
      trueSpeed = parseInt(payload.substring(2));
      document.getElementById('speedInput').value = trueSpeed;
      document.getElementById('speed').innerHTML = trueSpeed;
    }
    
  if(payloadArray[0] == "#" && payloadArray[1] == "p"){
    var check = payload.substring(2);
    switch (check){
      case "1":
        document.getElementById('noise').selected = true;
        document.getElementById('rainbow').selected = false;
        document.getElementById('matrix').selected = false;
        document.getElementById('fire').selected = false;
        document.getElementById('windows').selected = false;
        document.getElementById('baustrahler').selected = false;
        break;
      case "2":
        document.getElementById('rainbow').selected = true;
        document.getElementById('noise').selected = false;
        document.getElementById('matrix').selected = false;
        document.getElementById('fire').selected = false;
        document.getElementById('windows').selected = false;
        document.getElementById('baustrahler').selected = false;
        break;
      case "3":
        document.getElementById('matrix').selected = true;
        document.getElementById('noise').selected = false;
        document.getElementById('rainbow').selected = false;
        document.getElementById('fire').selected = false;
        document.getElementById('windows').selected = false;
        document.getElementById('baustrahler').selected = false;
        break;
      case "4":
        document.getElementById('matrix').selected = false;
        document.getElementById('noise').selected = false;
        document.getElementById('rainbow').selected = false;
        document.getElementById('fire').selected = true;
        document.getElementById('windows').selected = false;
        document.getElementById('baustrahler').selected = false;
        break;
      case "5":
        document.getElementById('matrix').selected = false;
        document.getElementById('noise').selected = false;
        document.getElementById('rainbow').selected = false;
        document.getElementById('fire').selected = false;
        document.getElementById('windows').selected = true;
        document.getElementById('baustrahler').selected = false;
        break;
      case "6":
        document.getElementById('matrix').selected = false;
        document.getElementById('noise').selected = false;
        document.getElementById('rainbow').selected = false;
        document.getElementById('fire').selected = false;
        document.getElementById('windows').selected = false;
        document.getElementById('baustrahler').selected = true;
        break;
    }
  }
}
  
  function onUpdate(event){
    brightUpdate();
  }
  
  function onLoad(event) {
    initWebSocket();
    initButton();
    initBrightness();
    initSpeed();
    initPattern();

    console.log('Loaded!');
  }
  
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  
  function initBrightness() {
      var brightnessSlider = document.getElementById('brightnessInput');
      var brightness = document.getElementById('bright');
      brightness.innerHTML = brightnessSlider.value;
      
      brightnessSlider.oninput = function() {
        brightness.innerHTML = this.value;
      }
      brightnessSlider.addEventListener('input', brightUpdate); 
      brightnessSlider.addEventListener('change', brightUpdate);
  }

  function initSpeed(){
    var speedSlider = document.getElementById('speedInput');
      var speedness = document.getElementById('speed');
      speedness.innerHTML = speedSlider.value;
      
      speedSlider.oninput = function() {
        speedness.innerHTML = this.value;
      }
      speedSlider.addEventListener('input', speedUpdate); 
      speedSlider.addEventListener('change', speedUpdate);
  }

  function initPattern(){
    list.addEventListener('change', patternUpdate);
  }
  
  function toggle(){    
    websocket.send('*toggle');
  }
  
  function brightUpdate(){
    var brightnessUpdate = document.getElementById('brightnessInput').value;
    var limit = 'brightness=' + brightnessUpdate;
    websocket.send('#brightUpdate='+brightnessUpdate);
  }

  function speedUpdate(){
    var speedUpdate = document.getElementById('speedInput').value;
    var limit = 'speed=' + speedUpdate;
    websocket.send('#speedUpdate='+speedUpdate);
  }

  function patternUpdate(){
    var pU;
    switch(list.value){
      case "noise":
        pU = 1;
        console.log(pU);
        websocket.send('#patternUpdate='+pU);
        break;
      case "rainbow":
        pU = 2;
        console.log(pU);
        websocket.send('#patternUpdate='+pU);
        break;
      case "matrix":
        pU = 3;
        console.log(pU);
        websocket.send('#patternUpdate='+pU);
        break;
      case "fire":
        pU = 4;
        console.log(pU);
        websocket.send('#patternUpdate='+pU);
        break;
      case "windows":
        pU = 5;
        console.log(pU);
        websocket.send('#patternUpdate='+pU);
        break;
      case "baustrahler":
        pU = 6;
        console.log(pU);
        websocket.send('#patternUpdate='+pU);
        break;
       default:  
        console.log(list.value);
        websocket.send('#patternUpdate='+pU);
        break;
    }
  }

</script>
</body>
</html>
)rawliteral";
