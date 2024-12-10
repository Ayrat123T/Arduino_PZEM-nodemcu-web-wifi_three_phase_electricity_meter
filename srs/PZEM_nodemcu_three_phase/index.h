const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="ru">
    <head>
        <title>Автоматическая проверка ИПУЭ</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="user-scalable=no">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, height=device-height, initial-scale=1.0" />
<style type="text/css">
html {
    height: 100%;
    margin: 0;
    -webkit-font-smoothing: antialiased;
    font-family: 'bahnschrift';
    text-align: center;
    font-size: 20pt;
}
body {
    background-color: #323131;
    color: white;
}
.header {
    align-items: baseline;
    justify-content: space-between;
    font-weight: bold;
    font-size: 22pt;
    float: center;
    text-align: center;
    display: flow-root;
}
.footer {
    font-size: 12pt;
    color: #ffffff;
    text-align: center;
    display: inline-block;
}
.footer:hover {
    opacity: 0.7;
  }
.output {
    font-size: 18pt;
    text-align: left;
}
.input {
    text-align: right;
    max-width: 150px;
    width: 30vw;
    font-size: 22pt;
    display: inline;
    font-family: 'bahnschrift';
}
input:hover {
    opacity: 0.7;
  }
.labelMeterCheck {
    text-align: center;
    font-size: 20pt;
    height: 100%;
    display: inline-table;
    font-family: 'bahnschrift';
}
.inputMeterCheck {
    text-align: right;
    max-width: 150px;
    width: 15vw;
    font-size: 22pt;
    display: inline-table;
    font-family: 'bahnschrift';
    cursor: default;
    opacity: 0.8;
}
.inputErr {
    text-align: center;
    font-size: 22pt;
    height: 100%;
    width: 200px;
    display: inline-block;
    font-family: 'bahnschrift';
    margin: 5px;
}
.inputRadio {
    font-size: 20pt;
}
.button {
    border-radius: 10px;
    height: 100%;
    margin: 1%;
    box-sizing: border-box;
    border: 3px;
    text-align: center;
    font-size: 22pt;
    cursor: pointer;
    color: white;
    background-color: rgb(0,145,215);
    font-family: 'bahnschrift';
}
.button:hover {
    opacity: 0.6;
  }
#StartMeterCheck {
    background-color: #77dd77;
    font-size: 30pt;
    padding:2%;
}
#resetMeterCheck {
    font-size: 30pt;    padding:2%
}
input[type="checkbox"] {
    font-size: 22;
    width: 20px;
    height: 20px;
}
</style>
    </head>
    <body class="main_body">
        <form name="MeterCheck">
            <p><table class="input"><tr><td>
                <label for="CurrentTransformerTransformationRatio">Ктт = </label>
                <input id="CurrentTransformerTransformationRatio" class="input" type="number" name="CurrentTransformerTransformationRatio" placeholder="1" value="1" required min="0" title="коэффициент трансформации трансформатора тока"/></td></tr></table>
                <table class="input"><tr><td><label for="ConstMeterImpsNumCheck" >A[имп/кВ*ч] = </label>
                    <input id="ConstMeterImpsNumCheck" class="input" type="number" name="ConstMeterImpsNumCheck" list = "impsList" required title="постоянная счётчика" min = "0"/></td></tr></table>
                    <datalist id="impsList">
                        <option value="250"></option>
                        <option value="500"></option>
                        <option value="1000"></option>
                        <option value="1600"></option>
                        <option value="3200"></option>
                        <option value="5000"></option>
                        <option value="10000"></option>
                    </datalist>
                <table class="input"><tr><td>
                    <input id="SMDSerialNumMeterCheck" class="inputErr" name="SMDSerialNumMeterCheck" placeholder="Номер ИПУ"/>
                </td></tr></table></p>
            <p><table class="input"><tr>
                    <td><label id="PzemVoltageMeterLabel" class="labelMeterCheck">U[В] = </label></td>
                    <td><input id="PzemVoltageMeterCheck1" class="inputMeterCheck" name="PzemVoltageMeterCheck1" required readonly/></td>
                    <td><input id="PzemVoltageMeterCheck2" type="number" class="inputMeterCheck" name="PzemVoltageMeterCheck2" required readonly/></td>
                    <td><input id="PzemVoltageMeterCheck3" type="number" class="inputMeterCheck" name="PzemVoltageMeterCheck3" required readonly/></td>
                </tr>
                <tr>
                    <td><label id="PzemCurrentMeterLabel" class="labelMeterCheck">I[А] = </label></td>
                    <td><input id="PzemCurrentMeterCheck1" type="number" class="inputMeterCheck" name="PzemCurrentMeterCheck1" required readonly/></td>
                    <td><input id="PzemCurrentMeterCheck2" type="number" class="inputMeterCheck" name="PzemCurrentMeterCheck2" required readonly/></td>
                    <td><input id="PzemCurrentMeterCheck3" type="number" class="inputMeterCheck" name="PzemCurrentMeterCheck3" required readonly/></td>
                </tr>
                <tr>
                    <td><label id="PzemPowerMeterLabel" class="labelMeterCheck">P[кВт] = </label></td>
                    <td><input id="PzemPowerMeterCheck1" type="number" class="inputMeterCheck" name="PzemPowerMeterCheck1" required readonly/></td>
                    <td><input id="PzemPowerMeterCheck2" type="number" class="inputMeterCheck" name="PzemPowerMeterCheck2" required readonly/></td>
                    <td><input id="PzemPowerMeterCheck3" type="number" class="inputMeterCheck" name="PzemPowerMeterCheck3" required readonly/></td>
                </tr>
                <tr>
                    <td><label id="PzemEnergyMeterLabel" class="labelMeterCheck">W[кВт*ч] = </label></td>
                    <td><input id="PzemEnergyMeterCheck1" type="number" class="inputMeterCheck" name="PzemEnergyMeterCheck1" required readonly/></td>
                    <td><input id="PzemEnergyMeterCheck2" type="number" class="inputMeterCheck" name="PzemEnergyMeterCheck2" required readonly/></td>
                    <td><input id="PzemEnergyMeterCheck3" type="number" class="inputMeterCheck" name="PzemEnergyMeterCheck3" required readonly/></td>
                </tr>
                <tr>
                    <td><label id="PzemFrequencyMeterLabel" class="labelMeterCheck">f[Гц] = </label></td>
                    <td><input id="PzemFrequencyMeterCheck1" type="number" class="inputMeterCheck" name="PzemFrequencyMeterCheck1" required readonly/></td>
                    <td><input id="PzemFrequencyMeterCheck2" type="number" class="inputMeterCheck" name="PzemFrequencyMeterCheck2" required readonly/></td>
                    <td><input id="PzemFrequencyMeterCheck3" type="number" class="inputMeterCheck" name="PzemFrequencyMeterCheck3" required readonly/></td>
                </tr>
                <tr>
                    <td><label id="PzemPowerFactorMeterLabel" class="labelMeterCheck">cos(ф) = </label></td>
                    <td><input id="PzemPowerFactorMeterCheck1" type="number" class="inputMeterCheck" name="PzemPowerFactorMeterCheck1" required readonly/></td>
                    <td><input id="PzemPowerFactorMeterCheck2" type="number" class="inputMeterCheck" name="PzemPowerFactorMeterCheck2" required readonly/></td>
                    <td><input id="PzemPowerFactorMeterCheck3" type="number" class="inputMeterCheck" name="PzemPowerFactorMeterCheck3" required readonly/></td>
                </tr>
            </table></p>
            <p class="input">
                <table class="input"><tr><td><label for ="PzemFullCurrentCheck"  id="PzemFullCurrentMeterLabel" class="labelMeterCheck">Iсум[А] = </label>
                    <input id="PzemFullCurrentCheck" type="number" class="inputMeterCheck" name="PzemFullCurrentCheck" required readonly/></td></tr></table>
                <table class="input"><tr><td><label for ="PzemFullPowerCheck" id="PzemFullPowerMeterLabel" class="labelMeterCheck">Pсум[кВт] = </label>
                    <input id="PzemFullPowerCheck" type="number" class="inputMeterCheck" name="PzemFullPowerCheck" required readonly/></td></tr></table>
                <table class="input"><tr><td><label for ="PzemFullEnergyMeterCheck" id="PzemFullEnergyMeterLabel" class="labelMeterCheck">Wсум[кВт*ч] = </label>
                    <input id="PzemFullEnergyMeterCheck" type="number" class="inputMeterCheck" name="PzemFullEnergyMeterCheck" required readonly/></td></tr></table>
             </p>
            <p><table class="input"><tr><td><label id="KYImpsMeterLabel" class="labelMeterCheck">n[имп] = </label>
                    <input id="KYImpsMeterCheck" type="number" class="inputMeterCheck" name="KYImpsMeterCheck"  title="кол-во импульсов" required readonly/></td></tr></table>
                <table class="input"><tr><td><label id="ImpsPeriodMeterLabel" class="labelMeterCheck">Tимп[с] = </label>
                    <input id="ImpsPeriodMeterCheck" type="number" class="inputMeterCheck" name="ImpsPeriodMeterCheck" title="период между импульсами" required readonly/></td></tr></table>
            </p>
            <p class="input"><span id="timer" class="input">00:00:00.0</span></p>
            <p><table class="input"><tr><td><label id="CalcMeterPowerLabel" class="labelMeterCheck">Pипэу[кВт*ч] = </label>
                <input id="CalcMeterPowerCheck" type="number" class="inputMeterCheck" name=CalcMeterPowerCheck"  title="мощность счётчика по импульсам" required readonly/></td></tr></table>
            <table class="input"><tr><td><label id="CalcMeterAccuracyLabel" class="labelMeterCheck">Погрешность[%] = </label>
                <input id="CalcMeterAccuracyCheck" type="number" class="inputMeterCheck" name="CalcMeterAccuracyCheck" title="Погрешность счётчика" required readonly/></td></tr></table>
             </p>
                <p><button id="StartMeterCheck" type="submit" class="button" name="StartMeterCheck">Старт▶</button>
                    <button type="reset" onclick="clearALL()" class="button" id="resetMeterCheck">↩︎ Сброс</button></p>
        </form>        <button id = "copyResultButton"        name = "copyResultButton"        class = "button"
        style="display: none">📋 Скопировать результат</button>
        <br>
        <div class="output" id="printBlock"></div>
        <div class="footer" id="CopyRights"><p><a href="https://gridcom-rt.ru/" class="footer">© 2024 GridCom</a></p>
            <a href="mailto:airattu@mail.ru" class="footer">📧 airattu@mail.ru</a></div>
<script>
let сurrentTransformerTransformationRatio = document.getElementById('CurrentTransformerTransformationRatio');
let constMeterImpsNumCheck = document.getElementById('ConstMeterImpsNumCheck');
let meterSerialNumMeterCheck = document.getElementById('SMDSerialNumMeterCheck');
let pzemVoltageMeterCheck1 = document.getElementById('PzemVoltageMeterCheck1');
let pzemVoltageMeterCheck2 = document.getElementById('PzemVoltageMeterCheck2');
let pzemVoltageMeterCheck3 = document.getElementById('PzemVoltageMeterCheck3');
let pzemCurrentMeterCheck1 = document.getElementById('PzemCurrentMeterCheck1');
let pzemCurrentMeterCheck2 = document.getElementById('PzemCurrentMeterCheck2');
let pzemCurrentMeterCheck3 = document.getElementById('PzemCurrentMeterCheck3');
let pzemPowerMeterCheck1 = document.getElementById('PzemPowerMeterCheck1');
let pzemPowerMeterCheck2 = document.getElementById('PzemPowerMeterCheck2');
let pzemPowerMeterCheck3 = document.getElementById('PzemPowerMeterCheck3');
let pzemEnergyMeterCheck1 = document.getElementById('PzemEnergyMeterCheck1');
let pzemEnergyMeterCheck2 = document.getElementById('PzemEnergyMeterCheck2');
let pzemEnergyMeterCheck3 = document.getElementById('PzemEnergyMeterCheck3');
let pzemFrequencyMeterCheck1 = document.getElementById('PzemFrequencyMeterCheck1');
let pzemFrequencyMeterCheck2 = document.getElementById('PzemFrequencyMeterCheck2');
let pzemFrequencyMeterCheck3 = document.getElementById('PzemFrequencyMeterCheck3');
let pzemPowerFactorMeterCheck1 = document.getElementById('PzemPowerFactorMeterCheck1');
let pzemPowerFactorMeterCheck2 = document.getElementById('PzemPowerFactorMeterCheck2');
let pzemPowerFactorMeterCheck3 = document.getElementById('PzemPowerFactorMeterCheck3');
let pzemFullCurrentCheck = document.getElementById('PzemFullCurrentCheck');
let pzemFullPowerCheck = document.getElementById('PzemFullPowerCheck');
let pzemFullEnergyMeterCheck = document.getElementById('PzemFullEnergyMeterCheck');
let kyImpsMeterCheck = document.getElementById('KYImpsMeterCheck');
let impsPeriodMeterCheck = document.getElementById('ImpsPeriodMeterCheck');
let timer = document.getElementById('timer');
let calcMeterPower = document.getElementById('CalcMeterPowerCheck');
let calcMeterAccuracy = document.getElementById('CalcMeterAccuracyCheck');
let seconds = 0.0;
let minutes = 0;
let hours = 0;
let interval;
let StartMeterCheckBtn = document.getElementById('StartMeterCheck');
let resetBtn = document.getElementById('resetMeterCheck');
StartMeterCheckBtn.addEventListener('click', startMeterCheck);
function CheckAllInputs() {    if (constMeterImpsNumCheck.value != '' &&        сurrentTransformerTransformationRatio.value >= 0 &&
        сurrentTransformerTransformationRatio.value != '') {
        return true;
    }
    return false;
};
function ViewAllESPdata(ESPdata) {
    pzemVoltageMeterCheck1.value = ESPdata.voltages[0];
    pzemVoltageMeterCheck2.value = ESPdata.voltages[1];
    pzemVoltageMeterCheck3.value = ESPdata.voltages[2];
    pzemCurrentMeterCheck1.value = ESPdata.currents[0];
    pzemCurrentMeterCheck2.value = ESPdata.currents[1];
    pzemCurrentMeterCheck3.value = ESPdata.currents[2];
    pzemPowerMeterCheck1.value = ESPdata.powers[0];
    pzemPowerMeterCheck2.value = ESPdata.powers[1];
    pzemPowerMeterCheck3.value = ESPdata.powers[2];
    pzemEnergyMeterCheck1.value = ESPdata.energies[0];
    pzemEnergyMeterCheck2.value = ESPdata.energies[1];
    pzemEnergyMeterCheck3.value = ESPdata.energies[2];
    pzemFrequencyMeterCheck1.value = ESPdata.frequencies[0];
    pzemFrequencyMeterCheck2.value = ESPdata.frequencies[1];
    pzemFrequencyMeterCheck3.value = ESPdata.frequencies[2];
    pzemPowerFactorMeterCheck1.value =ESPdata.powerFactories[0];
    pzemPowerFactorMeterCheck2.value =ESPdata.powerFactories[1];
    pzemPowerFactorMeterCheck3.value =ESPdata.powerFactories[2];
    pzemFullCurrentCheck.value = ESPdata.FullValues.current;
    pzemFullPowerCheck.value = ESPdata.FullValues.power;
    pzemFullEnergyMeterCheck.value = ESPdata.FullValues.energy;
    kyImpsMeterCheck.value = ESPdata.ResSMDValues.KYimpNumSumm;
    impsPeriodMeterCheck.value = ESPdata.ResSMDValues.SMDimpPeriod;
    calcMeterPower.value = ESPdata.ResSMDValues.SMDpower;
    calcMeterAccuracy.value = ESPdata.ResSMDValues.SMDAccuraty;
}
constMeterImpsNumCheck.addEventListener("change", sendConstMeterImpsNumCheck);
function sendConstMeterImpsNumCheck() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "const_meter_imps_num?constMeterImpsNumVal="+constMeterImpsNumCheck.value, true);
  xhttp.responseType = "json";
  xhttp.send();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
        console.log("sendConstMeterImpsNumCheck successful✔️\n\r");
        console.log(xhttp.response);
    } /*else {
        console.log("sendConstMeterImpsNumCheck fallied⛔️");    }*/
  };
  xhttp.onload = function () {
    ViewAllESPdata(xhttp.response);
  };
  delete xhttp;
};
сurrentTransformerTransformationRatio.addEventListener("change", sendCurrentTransformerTransformationRatio);
function sendCurrentTransformerTransformationRatio() {
    var xhttp = new XMLHttpRequest();    xhttp.open("GET",        "сurrent_transformer_transformation_ratio?сurrentTransformerTransformationRatio="+сurrentTransformerTransformationRatio.value,
        true);
    xhttp.responseType = "json";
    xhttp.send();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
          console.log("sendCurrentTransformerTransformationRatio successful✔️\n\r");
          console.log(xhttp.response);
      } /*else {
          console.log("sendCurrentTransformerTransformationRatio fallied⛔️");      }*/
    };
    xhttp.onload = function () {
        ViewAllESPdata(xhttp.response);
    };
};
function getPZEMsData() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "pzem_values");
    xhttp.responseType = "json";
    xhttp.send();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log("getPZEMsData successful✔️\n\r");
            console.log(xhttp.response);
        } /*else {
            console.log("sendCurrentTransformerTransformationRatio fallied⛔️");        }*/
      };
    xhttp.onload = function () {
        console.log(xhttp.response);
        ViewAllESPdata(xhttp.response);
    };
};
/*setInterval(function() {
  getPZEMsData();}, 1000);*/
function ResetPZEMs() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
        }
    };
    xhttp.open("GET", "pzem_reset_energies", true);
    xhttp.send();
};
function startMeterCheck(e) {
    if (CheckAllInputs()) {
        e.preventDefault();
        if (StartMeterCheckBtn.innerText == 'Старт▶') {
            if (сurrentTransformerTransformationRatio.value <= 0) {
                alert('Ктт не должен быть равен 0');
            } else {
                sendConstMeterImpsNumCheck();
                sendCurrentTransformerTransformationRatio();
                getPZEMsData();
                seconds = 0.0;
                minutes = 0;
                hours = 0;
                energy = 0.0;
                interval = setInterval(updateTime, 100);
                StartMeterCheckBtn.innerText = 'Стоп⛔️';
                StartMeterCheckBtn.style["background-color"] = "red";
                writeBtn.style["display"] = "none";
            }
        } else {
            clearInterval(interval);
            StartMeterCheckBtn.innerText = 'Старт▶';
            StartMeterCheckBtn.style["background-color"] = "#77dd77";
            calcMeterAccuracyAndShowRes(e);
        }
    }
};
function updateTime() {
    seconds += 0.1;
    if (seconds >= 60.0) {
        minutes++;
        seconds = 0.0;
    }
    if (minutes == 60) {
        hours++;
        minutes = 0;
    }
    timer.textContent = `${hours.toString().padStart(2, '0')}:${minutes.toString().padStart(2, '0')}:${seconds.toFixed(1).toString().padStart(2, '0')}`;
};
resetBtn.addEventListener('click', () => {
    clearInterval(interval);
    ResetPZEMs();
    seconds = 0.0;
    minutes = 0;
    hours = 0;
    timer.textContent = '00:00:00.0';
    StartMeterCheckBtn.innerText = 'Старт▶';
    StartMeterCheckBtn.style["background-color"] = "#77dd77";
});
function calcMeterAccuracyAndShowRes(e) {
    writeBtn.style["display"] = "";
    clearInterval(interval);
    StartMeterCheckBtn.innerText = 'Старт▶';
    StartMeterCheckBtn.style["background-color"] = "#77dd77";
};
function clearALL() {
    ResetPZEMs();
    writeBtn.style.display = "none";
};
const writeBtn = document.getElementById("copyResultButton");
writeBtn.addEventListener("click", copyResult);
function copyResult(e) {
    e.preventDefault();
    var inp = document.createElement('input');
        var now = new Date();        inp.value = now + ';    \n\r\Номер ИПУ: ' + document.getElementById('SMDSerialNumMeterCheck').value + ';    \n\r\Время: ' + timer.textContent + ';    \n\r\Ктт = ' + сurrentTransformerTransformationRatio.value + ' о.е.;    \n\r\U1 = ' + pzemVoltageMeterCheck1.value.toString() + ' В;    \n\r\I1 = ' + pzemCurrentMeterCheck1.value.toString() + ' A;    \n\r\U2 = ' + pzemVoltageMeterCheck2.value.toString() + ' В;    \n\r\I2 = ' + pzemCurrentMeterCheck2.value.toString() + ' A;    \n\r\U3 = ' + pzemVoltageMeterCheck3.value.toString() + ' В;    \n\r\I3 = ' + pzemCurrentMeterCheck3.value.toString() + ' A;    \n\r'+ calcMeterPower.textContent + ';    \n\r'+ calcMeterEnergy.textContent + ';    \n\r\A = ' + constMeterImpsNumCheck.value.toString() + ' имп/кВ*ч;   \n\r\
n = ' + kyImpsMeterCheck.value.toString() + ' имп;    \n\r';
    document.body.appendChild(inp);
    inp.select();
    if (document.execCommand('copy')) {
        if (writeBtn.innerText !== 'Скопировано!') {
            const originalText = writeBtn.innerText;
            writeBtn.innerText = 'Скопировано!';
            setTimeout(() => {
                writeBtn.innerText = originalText;
            }, 1500);
        }
    } else {
        console.log("Failed...");
    }
    document.body.removeChild(inp);
};
</script>
    </body>
</html>)=====";