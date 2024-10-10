#ifndef PAGEHTML_HPP
#define PAGEHTML_HPP


const char LOGIN_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; background-color: #DCDCDC; }
.login-container { position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%);
                   width: 300px; padding: 20px; background-color: #C0C0C0; border-radius: 10px;  box-shadow: 0px 2px 2px 0px rgba(0, 0, 0, 0.2); }
input[type=text], input[type=password] { width: 100%; padding: 12px 20px; margin: 8px 0; display: inline-block;
                                         border: 1px solid #C0C0C0; border-radius: 10px; box-sizing: border-box; background: #DCDCDC; }
input[type=submit] { width: 100%; background-color: #4CAF50; color: white; padding: 14px 20px; margin: 8px 0;
                    border: none; border-radius: 10px; cursor: pointer; }
input[type=submit]:hover { background-color: #45a049; }
</style>
</head>
<body>
<div class="login-container">
<h2>Login</h2>
<form action="/login" method="POST">
<input type="text" name="username" placeholder="Usuário">
<br>
<input type="password" name="password" placeholder="Senha">
<br>
<input type="submit" value="Entrar">
</form>
</div>
</body>
</html>
)rawliteral";


//Pagina html da tela principal
const char HOME_PAGE[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body { 
  font-family: Arial, sans-serif; 
  text-align: center; 
  margin: 0; 
  padding: 0; 
  background-color: #A9A9A9;
}

.header-container { 
  display: flex; 
  align-items: center; 
  justify-content: space-between; 
  padding: 10px 20px;
  position: relative;
}

.header-left { 
  font-weight: bold;
  font-size: 24px; 
  color: #333; 
}

.header-right button { 
  background-color: #D3D3D3; 
  border: none; 
  border-radius: 10px; 
  padding: 10px; 
  width: 80px; 
  font-size: 18px;
  cursor: pointer; 
  box-shadow: 0px 2px 2px 0px rgba(0, 0, 0, 0.2);
}

.temp-sensor { 
 display: flex; 
  justify-content: center; 
  padding: 10px; 
  margin-bottom: 5px; 
}

.temp-sensor-item { 
  background-color: #D3D3D3;
  padding: 5px;
  margin: 10px;
  flex: 1;
  max-width: 50%;
  text-align: center;
  border-radius: 10px;
  box-shadow: 0px 2px 2px 0px rgba(0, 0, 0, 0.2);
}

.grid { 
  display: grid; 
  grid-template-columns: repeat(2, 1fr); 
  grid-auto-rows: 180px; 
  gap: 2px; 
  justify-items: center;
  padding: 10px;
  margin: 20px;
  background-color: #D3D3D3; 
  border-radius: 10px;  
  box-shadow: 0px 2px 2px 0px rgba(0, 0, 0, 0.2);
}

.grid-item { 
  display: flex; 
  flex-direction: column;
  align-items: center;
  justify-content: center;
  width: 100%; 
}

.grid-item p {
  font-size: 18px; 
  margin-bottom: 10px; 
  margin-top: 0; 
  margin-bottom: 50px;
}

.switch {
  position: relative;
  display: inline-block;
  width: 120px;
  height: 60px;
  transform: rotate(-90deg);
}

.switch input { 
  opacity: 0;
  width: 0;
  height: 0;
}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #808080;
  transition: .4s;
  border-radius: 10px;
}

.slider:before {
  position: absolute;
  content: "";
  height: 52px;
  width: 52px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  transition: .4s;
  border-radius: 10px;
}

input:checked + .slider {
 background-color: #f9d835;
 box-shadow: 0 0 10px #f9d835;
}

input:checked + .slider:before {
  transform: translateX(60px);
}

/* Mensagem de erro de conexão */
#connection-status {
  color: red;
  font-weight: bold;
  display: none;
}
</style>
</head>
<body>

<div class="header-container">
  <div class="header-left">ESP SH v1</div>
  <div class="header-right">
    <button onclick="location.href='/logout'">
      Sair
    </button>
  </div>
</div>

<!-- Mensagem de conexão -->
<p id="connection-status">❌ Conexão perdida com o servidor!</p>

<!-- Sensor readings -->
<div class="temp-sensor">
  <div class="temp-sensor-item">
    <p>🌡️Temperatura</p>  
    <p><span id="temperature">%TEMPERATURE%</span> °C</p>
  </div>
  <div class="temp-sensor-item">
    <p>💧Umidade</p>
    <p><span id="humidity">%HUMIDITY%</span> %</p>
  </div>
</div>

<!-- Grid para toggle buttons -->
<div class="grid">
  <div class="grid-item">
    <p>Dispositivo 1</p>
    <label class="switch">
      <input type="checkbox" id="toggleBtn0" onchange="toggleDevice(0)">
      <span class="slider"></span>
    </label>
  </div>
  
  <div class="grid-item">
    <p>Dispositivo 2</p>
    <label class="switch">
      <input type="checkbox" id="toggleBtn1" onchange="toggleDevice(1)">
      <span class="slider"></span>
    </label>
  </div>

  <div class="grid-item">
    <p>Dispositivo 3</p>
    <label class="switch">
      <input type="checkbox" id="toggleBtn2" onchange="toggleDevice(2)">
      <span class="slider"></span>
    </label>
  </div>

  <div class="grid-item">
    <p>Dispositivo 4</p>
    <label class="switch">
      <input type="checkbox" id="toggleBtn3" onchange="toggleDevice(3)">
      <span class="slider"></span>
    </label>
  </div>
</div>

<script>
let ws = new WebSocket('ws://' + window.location.hostname + ':81');
let isConnected = false;

ws.onopen = function() {
  isConnected = true;
  updateConnectionStatus();
};

ws.onclose = function() {
  isConnected = false;
  updateConnectionStatus();
};

ws.onerror = function() {
  isConnected = false;
  updateConnectionStatus();
};

ws.onmessage = function(event) {
  let data = JSON.parse(event.data);
  
  // Atualiza os estados dos botões de acordo com os dados recebidos
  for (let i = 0; i < %NUM_DEVICES%; i++) {
    let btn = document.getElementById('toggleBtn' + i);
    btn.checked = data.deviceStates[i]; // Atualiza o toggle
    btn.disabled = !isConnected; // Desabilita os botões se não estiver conectado
  }
  document.getElementById('temperature').textContent = data.temperature.toFixed(1);
  document.getElementById('humidity').textContent = data.humidity.toFixed(1);
};

// Atualiza o status de conexão
function updateConnectionStatus() {
  const statusMessage = document.getElementById('connection-status');
  if (isConnected) {
    statusMessage.style.display = 'none';
    enableAllButtons();
  } else {
    statusMessage.style.display = 'block';
    disableAllButtons();
  }
}

// Função para desabilitar todos os botões
function disableAllButtons() {
  for (let i = 0; i < %NUM_DEVICES%; i++) {
    document.getElementById('toggleBtn' + i).disabled = true;
  }
}

// Função para habilitar todos os botões
function enableAllButtons() {
  for (let i = 0; i < %NUM_DEVICES%; i++) {
    document.getElementById('toggleBtn' + i).disabled = false;
  }
}

// Função para enviar comando quando botão é alterado
function toggleDevice(index) {
  if (isConnected) {
    ws.send(index.toString()); // Envia o índice do botão para o ESP32
  } else {
    alert('Conexão perdida! Tente novamente quando a conexão for restabelecida.');
  }
}
</script>
</body>
</html>



)rawliteral";


#endif // PAGEHTML_HPP