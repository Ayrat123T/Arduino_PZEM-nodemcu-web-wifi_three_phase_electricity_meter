const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="ru">
    <head>
        <title>Энергомонитор SmartGridComMeter</title>
        <meta charset="UTF-8">
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
                <label for="CurrentTransformerTransformationRatioCheck">Ктт = </label>
                <input id="CurrentTransformerTransformationRatioCheck" class="input" type="number" name="CurrentTransformerTransformationRatioCheck" placeholder="1" value="1" required min="0" title="коэффициент трансформации трансформатора тока"/></td></tr></table>
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
                <table class="input"><tr><td><label id="QueueSizeCalcMeterAccuracyLabel" class="input">Кол-во импульсов = </label>
                    <input id="QueueSizeCalcMeterAccuracyCheck" type="number" class="input" name="QueueSizeCalcMeterAccuracyCheck" title="Кол-во последовательных импульсов, которые будут учитываться при расчёте погрешности" value="1" min = "0" required/></td></tr></table>
                <table class="input"><tr><td>
                    <input id="SMDSerialNumMeterCheck" class="inputErr" name="SMDSerialNumMeterCheck" placeholder="Номер ИПУ" title="будет сохранён в csv"/>
                </td></tr></table>
            </p>
            <p><table class="input"><tr>
                    <td><label id="PzemVoltageMeterLabel" class="labelMeterCheck">U[В] = </label></td>
                    <td><input id="PzemVoltageMeterCheck1" type="number" class="inputMeterCheck" name="PzemVoltageMeterCheck1" required readonly title="напряжение фазы А"/></td>
                    <td><input id="PzemVoltageMeterCheck2" type="number" class="inputMeterCheck" name="PzemVoltageMeterCheck2" required readonly title="напряжение фазы B"/></td>
                    <td><input id="PzemVoltageMeterCheck3" type="number" class="inputMeterCheck" name="PzemVoltageMeterCheck3" required readonly title="напряжение фазы C"/></td>
                </tr>
                <tr>
                    <td><label id="PzemCurrentMeterLabel" class="labelMeterCheck">I[А] = </label></td>
                    <td><input id="PzemCurrentMeterCheck1" type="number" class="inputMeterCheck" name="PzemCurrentMeterCheck1" required readonly title="ток фазы А"/></td>
                    <td><input id="PzemCurrentMeterCheck2" type="number" class="inputMeterCheck" name="PzemCurrentMeterCheck2" required readonly title="ток фазы B"/></td>
                    <td><input id="PzemCurrentMeterCheck3" type="number" class="inputMeterCheck" name="PzemCurrentMeterCheck3" required readonly title="ток фазы C"/></td>
                </tr>
                <tr>
                    <td><label id="PzemPowerMeterLabel" class="labelMeterCheck">P[кВт] = </label></td>
                    <td><input id="PzemPowerMeterCheck1" type="number" class="inputMeterCheck" name="PzemPowerMeterCheck1" required readonly title="мощность фазы А"/></td>
                    <td><input id="PzemPowerMeterCheck2" type="number" class="inputMeterCheck" name="PzemPowerMeterCheck2" required readonly title="мощность фазы B"/></td>
                    <td><input id="PzemPowerMeterCheck3" type="number" class="inputMeterCheck" name="PzemPowerMeterCheck3" required readonly title="мощность фазы C"/></td>
                </tr>
                <tr>
                    <td><label id="PzemEnergyMeterLabel" class="labelMeterCheck">W[кВт*ч] = </label></td>
                    <td><input id="PzemEnergyMeterCheck1" type="number" class="inputMeterCheck" name="PzemEnergyMeterCheck1" required readonly title="Энергия фазы А"/></td>
                    <td><input id="PzemEnergyMeterCheck2" type="number" class="inputMeterCheck" name="PzemEnergyMeterCheck2" required readonly title="Энергия фазы B"/></td>
                    <td><input id="PzemEnergyMeterCheck3" type="number" class="inputMeterCheck" name="PzemEnergyMeterCheck3" required readonly title="Энергия фазы C"/></td>
                </tr>
                <tr>
                    <td><label id="PzemFrequencyMeterLabel" class="labelMeterCheck">f[Гц] = </label></td>
                    <td><input id="PzemFrequencyMeterCheck1" type="number" class="inputMeterCheck" name="PzemFrequencyMeterCheck1" required readonly title="частота фазы А"/></td>
                    <td><input id="PzemFrequencyMeterCheck2" type="number" class="inputMeterCheck" name="PzemFrequencyMeterCheck2" required readonly title="частота фазы B"/></td>
                    <td><input id="PzemFrequencyMeterCheck3" type="number" class="inputMeterCheck" name="PzemFrequencyMeterCheck3" required readonly title="частота фазы C"/></td>
                </tr>
                <tr>
                    <td><label id="PzemPowerFactorMeterLabel" class="labelMeterCheck">cos(ф) = </label></td>
                    <td><input id="PzemPowerFactorMeterCheck1" type="number" class="inputMeterCheck" name="PzemPowerFactorMeterCheck1" required readonly title="коэффициент мощности фазы А"/></td>
                    <td><input id="PzemPowerFactorMeterCheck2" type="number" class="inputMeterCheck" name="PzemPowerFactorMeterCheck2" required readonly title="коэффициент мощности фазы B"/></td>
                    <td><input id="PzemPowerFactorMeterCheck3" type="number" class="inputMeterCheck" name="PzemPowerFactorMeterCheck3" required readonly title="коэффициент мощности фазы C"/></td>                </tr> 
            </table></p>
            <p class="input">
                <table class="input"><tr><td><label for ="PzemFullCurrentCheck"  id="PzemFullCurrentMeterLabel" class="labelMeterCheck">Iсум[А] = </label>
                    <input id="PzemFullCurrentCheck" type="number" class="inputMeterCheck" name="PzemFullCurrentCheck" required readonly title="суммарный ток"/></td></tr></table>
                <table class="input"><tr><td><label for ="PzemFullPowerCheck" id="PzemFullPowerMeterLabel" class="labelMeterCheck">Pсум[кВт] = </label>
                    <input id="PzemFullPowerCheck" type="number" class="inputMeterCheck" name="PzemFullPowerCheck" required readonly title="суммарная мощность"/></td></tr></table>
                <table class="input"><tr><td><label for ="PzemFullEnergyMeterCheck" id="PzemFullEnergyMeterLabel" class="labelMeterCheck">Wсум[кВт*ч] = </label>
                    <input id="PzemFullEnergyMeterCheck" type="number" class="inputMeterCheck" name="PzemFullEnergyMeterCheck" required readonly title="суммарная энергия"/></td></tr></table>
             </p>
            <p><table class="input"><tr><td><label id="KYImpsMeterLabel" class="labelMeterCheck">n[имп] = </label>
                    <input id="KYImpsMeterCheck" type="number" class="inputMeterCheck" name="KYImpsMeterCheck"  title="кол-во импульсов" required readonly/></td></tr></table>
                <table class="input"><tr><td><label id="ImpsPeriodMeterLabel" class="labelMeterCheck">Tимп[с] = </label>
                    <input id="ImpsPeriodMeterCheck" type="number" class="inputMeterCheck" name="ImpsPeriodMeterCheck" title="период между импульсами" required readonly/></td></tr></table>
            </p>
            <p class="input"><span id="timer" class="input">00:00:00.0</span></p>
            <p><table class="input"><tr><td><label id="CalcMeterPowerLabel" class="labelMeterCheck">Pипэу[кВт] = </label>
                <input id="CalcMeterPowerCheck" type="number" class="inputMeterCheck" name=CalcMeterPowerCheck"  title="мощность счётчика по импульсам" required readonly/></td></tr></table>
            <table class="input"><tr><td><label id="CalcMeterAccuracyLabel" class="labelMeterCheck">Погрешность[%] = </label>
                <input id="CalcMeterAccuracyCheck" type="number" class="inputMeterCheck" name="CalcMeterAccuracyCheck" title="Погрешность счётчика" required readonly/></td></tr></table>
             </p>
                <p><button id="StartMeterCheck" type="submit" class="button" name="StartMeterCheck">Старт▶</button>
                    <button type="reset" class="button" id="resetMeterCheck">↩︎ Сброс</button></p>
        </form>        <button id = "copyResultButton"        name = "copyResultButton"        class = "button"
        style="display: none">📋 Скопировать результат</button>        <button id = "downloadCsvButton"        name = "downloadCsvButton"        class = "button"
        style="display: none">💾 Экспорт в csv</button>
        <br>
        <div class="output" id="printBlock"></div>
        <div class="footer" id="CopyRights"><p><a href="https://gridcom-rt.ru/" class="footer">© 2024 GridCom</a></p>
            <a href="mailto:airattu@mail.ru" class="footer">📧 airattu@mail.ru</a></div>
<script>
let currentTransformerTransformationRatioCheck = document.getElementById('CurrentTransformerTransformationRatioCheck');
let constMeterImpsNumCheck = document.getElementById('ConstMeterImpsNumCheck');
let queueSizeCalcMeterAccuracyCheck = document.getElementById('QueueSizeCalcMeterAccuracyCheck');
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
let PZEMinterval;
let timerInterval;let ESPsurveyPeriod = 1000; // период опроса ESP
let StartMeterCheckBtn = document.getElementById('StartMeterCheck');
let resetBtn = document.getElementById('resetMeterCheck');
StartMeterCheckBtn.addEventListener('click', startMeterCheck);
function CheckConstMeterImpsNumInputs() {    if (constMeterImpsNumCheck.value == ''
        || constMeterImpsNumCheck.value <= 0) {
            alert('A[имп/кВ*ч] должна быть больше 0');
            return false;
        }
    return true;
}
function CheckCurrentTransformerTransformationRatioInputs() {    if (currentTransformerTransformationRatioCheck.value == ''
        || currentTransformerTransformationRatioCheck.value <= 0) {
            alert('Ктт должен быть больше 0');
            return false;
        }
    return true;
};
function CheckQueueSizeCalcMeterAccuracyCheck() {    if (queueSizeCalcMeterAccuracyCheck.value == ''
        || queueSizeCalcMeterAccuracyCheck.value <= 0) {
            alert('Кол-во последовательных импульсов, которые будут учитываться при расчёте погрешности, должно быть больше 0');
            return false;
        }
    return true;
}let allValuesToCSV = [[    'time',    'timer',    'currentTransformerTransformationRatio',    'constMeterImpsNum',    'queueSizeCalcMeterAccuracy',    'meterSerialNum',    'pzemVoltage1',    'pzemVoltage2',    'pzemVoltage3',    'pzemCurrent1',    'pzemCurrent2',    'pzemCurrent3',    'pzemPower1',    'pzemPower2',    'pzemPower3',    'pzemEnergy1',    'pzemEnergy2',    'pzemEnergy3',    'pzemFrequency1',    'pzemFrequency2',    'pzemFrequency3',    'pzemPowerFactor1',    'pzemPowerFactor2',    'pzemPowerFactor3',    'pzemFullCurrent',    'pzemFullPower',    'pzemFullEnergy',    'kyImpsMeter',    'impsPeriodMeter',    'calcMeterPower',
    'calcMeterAccuracy']];
function ViewAllESPdata(ESPdata) {
    if (ESPdata.voltages[0]) {
        pzemVoltageMeterCheck1.value = ESPdata.voltages[0].toFixed(2);
        pzemCurrentMeterCheck1.value = ESPdata.currents[0].toFixed(2);
        pzemPowerMeterCheck1.value = ESPdata.powers[0].toFixed(2);
        pzemEnergyMeterCheck1.value = ESPdata.energies[0].toFixed(2);
        pzemFrequencyMeterCheck1.value = ESPdata.frequencies[0].toFixed(2);
        pzemPowerFactorMeterCheck1.value =ESPdata.powerFactories[0].toFixed(2);
    }
    if (ESPdata.voltages[1]) {
        pzemVoltageMeterCheck2.value = ESPdata.voltages[1].toFixed(2);
        pzemCurrentMeterCheck2.value = ESPdata.currents[1].toFixed(2);
        pzemPowerMeterCheck2.value = ESPdata.powers[1].toFixed(2);
        pzemEnergyMeterCheck2.value = ESPdata.energies[1].toFixed(2);
        pzemFrequencyMeterCheck2.value = ESPdata.frequencies[1].toFixed(2);
        pzemPowerFactorMeterCheck2.value =ESPdata.powerFactories[1].toFixed(2);
    }
    if (ESPdata.voltages[2]) {
        pzemVoltageMeterCheck3.value = ESPdata.voltages[2].toFixed(2);
        pzemCurrentMeterCheck3.value = ESPdata.currents[2].toFixed(2);
        pzemPowerMeterCheck3.value = ESPdata.powers[2].toFixed(2);
        pzemEnergyMeterCheck3.value = ESPdata.energies[2].toFixed(2);
        pzemFrequencyMeterCheck3.value = ESPdata.frequencies[2].toFixed(2);
        pzemPowerFactorMeterCheck3.value =ESPdata.powerFactories[2].toFixed(2);
    }
    if (ESPdata.FullValues.current) pzemFullCurrentCheck.value = ESPdata.FullValues.current.toFixed(2);
    if (ESPdata.FullValues.power) pzemFullPowerCheck.value = ESPdata.FullValues.power.toFixed(2);
    if (ESPdata.FullValues.energy) pzemFullEnergyMeterCheck.value = ESPdata.FullValues.energy.toFixed(2);
    kyImpsMeterCheck.value = ESPdata.ResSMDValues.KYimpNumSumm;
    if (ESPdata.ResSMDValues.SMDimpPeriod) impsPeriodMeterCheck.value = ESPdata.ResSMDValues.SMDimpPeriod.toFixed(2);
    if (ESPdata.ResSMDValues.SMDpower) calcMeterPower.value = ESPdata.ResSMDValues.SMDpower.toFixed(2);
    if (ESPdata.ResSMDValues.SMDAccuracy) calcMeterAccuracy.value = ESPdata.ResSMDValues.SMDAccuracy.toFixed(2);
    pushAllESPdataToCSVarray();
}
function formatDate(date) {    return date.getFullYear() + '/' +      (date.getMonth() + 1) + '/' +      date.getDate() + ' ' +      date.getHours() + ':' +      date.getMinutes() + ':' +
      date.getSeconds() + ' ';
   }
const downloadCsvBtn = document.getElementById("downloadCsvButton");
downloadCsvBtn.addEventListener("click", DownloadCsv);
function DownloadCsv() {        const csvContent = allValuesToCSV.map(row =>             row.map(field =>                 `"${field.toString().replace(".",",").replace(/"/g, '""')}"`            ).join(";")
        ).join("\n");    
        const blob = new Blob([csvContent], { type: 'text/csv;charset=utf-8;' });
        const url = URL.createObjectURL(blob);
        const link = document.createElement('a');
        link.href = url;
        const date = new Date();
        link.download = formatDate(date) + 'exportSmartGridComMeterData.csv';
        link.style.display = 'none';    
        document.body.appendChild(link);
        link.click();
        document.body.removeChild(link);
};
function pushAllESPdataToCSVarray() {
    downloadCsvBtn.style["display"] = "";
    var now = new Date();    allValuesToCSV.push([        formatDate(now),        timer.textContent,        currentTransformerTransformationRatioCheck.value,        constMeterImpsNumCheck.value,        queueSizeCalcMeterAccuracyCheck.value,        meterSerialNumMeterCheck.value,        pzemVoltageMeterCheck1.value,        pzemVoltageMeterCheck2.value,        pzemVoltageMeterCheck3.value,        pzemCurrentMeterCheck1.value,        pzemCurrentMeterCheck2.value,        pzemCurrentMeterCheck3.value,        pzemPowerMeterCheck1.value,        pzemPowerMeterCheck2.value,        pzemPowerMeterCheck3.value,        pzemEnergyMeterCheck1.value,        pzemEnergyMeterCheck2.value,        pzemEnergyMeterCheck3.value,        pzemFrequencyMeterCheck1.value,        pzemFrequencyMeterCheck2.value,        pzemFrequencyMeterCheck3.value,        pzemPowerFactorMeterCheck1.value,        pzemPowerFactorMeterCheck2.value,        pzemPowerFactorMeterCheck3.value,        pzemFullCurrentCheck.value,        pzemFullPowerCheck.value,        pzemFullEnergyMeterCheck.value,        kyImpsMeterCheck.value,        impsPeriodMeterCheck.value,        calcMeterPower.value,
        calcMeterAccuracy.value]);
}
currentTransformerTransformationRatioCheck.addEventListener("change", sendCurrentTransformerTransformationRatio);
function sendCurrentTransformerTransformationRatio() {
    if (CheckCurrentTransformerTransformationRatioInputs()) {
        var xhttp = new XMLHttpRequest();        xhttp.open("POST",            "current_transformer_transformation_ratio?currentTransformerTransformationRatio="+currentTransformerTransformationRatioCheck.value,
            true);
        xhttp.setRequestHeader("Content-Type", "text; charset=UTF-8");
        xhttp.send();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                console.log("sendCurrentTransformerTransformationRatio successful✔️\n\r");
                console.log(this.responseText);
            }
        };
    }
};
constMeterImpsNumCheck.addEventListener("change", sendConstMeterImpsNumCheck);
function sendConstMeterImpsNumCheck() {
    if (CheckConstMeterImpsNumInputs()) {
        var xhttp = new XMLHttpRequest();
        xhttp.open("POST", "const_meter_imps_num?constMeterImpsNumVal="+constMeterImpsNumCheck.value, true);
        xhttp.setRequestHeader("Content-Type", "text; charset=UTF-8");
        xhttp.send();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                console.log(this.responseText);
                console.log("sendConstMeterImpsNumCheck successful✔️\n\r");
            }
        };
    }
};
queueSizeCalcMeterAccuracyCheck.addEventListener("change", sendQueueSizeCalcMeterAccuracyCheck);
function sendQueueSizeCalcMeterAccuracyCheck() {
    if (CheckQueueSizeCalcMeterAccuracyCheck()) {
        var xhttp = new XMLHttpRequest();        xhttp.open("POST",            "queue_size_calc_meter_accuracy?queueSizeCalcMeterAccuracy="+queueSizeCalcMeterAccuracyCheck.value,
            true);
        xhttp.setRequestHeader("Content-Type", "text; charset=UTF-8");
        xhttp.send();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                console.log("sendQueueSizeCalcMeterAccuracyCheck successful✔️\n\r");
                console.log(this.responseText);
            }
        };
    }
};
function getPZEMsData() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "pzem_values", true);
    xhttp.responseType = "json";
    xhttp.send();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
          console.log("getPZEMsData successful✔️\n\r");
      }
    };
    xhttp.onload = function () {
        ViewAllESPdata(xhttp.response);
    };
};
function Reset() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "reset", true);
    xhttp.responseType = "text";
    xhttp.send();
    xhttp.onload = function () {
        console.log(this.responseText);
    };
};
function startMeterCheck(e) {
    if (StartMeterCheckBtn.innerText == 'Старт▶') {        if (CheckConstMeterImpsNumInputs()            && CheckCurrentTransformerTransformationRatioInputs()
            && CheckQueueSizeCalcMeterAccuracyCheck()) {
            e.preventDefault();
            PZEMinterval = setInterval(getPZEMsData, ESPsurveyPeriod);
            timerInterval = setInterval(updateTime, 100);
            StartMeterCheckBtn.innerText = 'Стоп⛔️';
            StartMeterCheckBtn.style["background-color"] = "red";
            writeBtn.style["display"] = "none";            
        }
    } else {
        e.preventDefault();
        clearInterval(PZEMinterval);
        clearInterval(timerInterval);
        StartMeterCheckBtn.innerText = 'Старт▶';
        StartMeterCheckBtn.style["background-color"] = "#77dd77";
        writeBtn.style["display"] = "";
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
    clearInterval(PZEMinterval);
    setTimeout(() => {
        console.log("ждём, пока придут последние значения с pzem");
      }, ESPsurveyPeriod + 100);
    clearInterval(timerInterval);
    Reset();
    seconds = 0.0;
    minutes = 0;
    hours = 0;
    timer.textContent = '00:00:00.0';
    StartMeterCheckBtn.innerText = 'Старт▶';
    StartMeterCheckBtn.style["background-color"] = "#77dd77";
    writeBtn.style.display = "none";
});
const writeBtn = document.getElementById("copyResultButton");
writeBtn.addEventListener("click", copyResult);
function copyResult(e) {
    e.preventDefault();
    var inp = document.createElement('input');
        var now = new Date();        inp.value = now + ';    \n\r\Номер ИПУ: ' + document.getElementById('SMDSerialNumMeterCheck').value + ';    \n\r\Время: ' + timer.textContent + ';    \n\r\Ктт = ' + currentTransformerTransformationRatioCheck.value + ' о.е.;    \n\r\U1 = ' + pzemVoltageMeterCheck1.value.toString() + ' В;    \n\r\I1 = ' + pzemCurrentMeterCheck1.value.toString() + ' A;    \n\r\U2 = ' + pzemVoltageMeterCheck2.value.toString() + ' В;    \n\r\I2 = ' + pzemCurrentMeterCheck2.value.toString() + ' A;    \n\r\U3 = ' + pzemVoltageMeterCheck3.value.toString() + ' В;    \n\r\I3 = ' + pzemCurrentMeterCheck3.value.toString() + ' A;    \n\r'+ calcMeterPower.textContent + ';    \n\r\A = ' + constMeterImpsNumCheck.value.toString() + ' имп/кВ*ч;   \n\r\
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