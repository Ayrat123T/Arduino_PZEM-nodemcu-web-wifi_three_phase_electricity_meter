let сurrentTransformerTransformationRatio = document.getElementById('CurrentTransformerTransformationRatio');

let constMeterImpsNumCheck = document.getElementById('ConstMeterImpsNumCheck');

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

let kyImpsMeterCheck = document.getElementById('KYImpsMeterCheck');

let impsPeriodMeterCheck = document.getElementById('ImpsPeriodMeterCheck');

const StartMeterCheckBtn = document.MeterCheck.StartMeterCheck;
StartMeterCheckBtn.addEventListener('click', startMeterCheck);

function CheckAllInputs() {
    if (impsMeterCheck.value != '' &&
        сurrentTransformerTransformationRatio.value >= 0 &&
        сurrentTransformerTransformationRatio.value != '') {
        return true;
    }
    return false;
}

function sendConstMeterImpsNumCheck() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
        constMeterImpsNumCheck.value = this.responseText;
    }
  };
  xhttp.open("GET", "led_set?constMeterImpsNumVal="+constMeterImpsNumCheck.value, true);
  xhttp.send();
}

function getPZEMsData() {
    function getData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            pzemVoltageMeterCheck1.value = this.response.data.voltages[0];
            pzemVoltageMeterCheck2.value = this.response.data.voltages[1];
            pzemVoltageMeterCheck3.value = this.response.data.voltages[2];
          }
        };
        xhttp.open("GET", "pzem_values", true);
        xhttp.send();
      }
}

setInterval(function() 
{
  getPZEMsData();
}, 1000);

function ResetPZEMs() {

}

function startMeterCheck(e) {
    if (CheckAllInputs()) {
        e.preventDefault();
        if (StartMeterCheckBtn.innerText == 'Старт▶') {
            if (сurrentTransformerTransformationRatio.value <= 0) {
                alert('Ктт не должен быть равен 0');
            } else {
                SetImpSMDсonst();
                ResetPZEMs();
                seconds = 0.0;
                minutes = 0;
                hours = 0;
                energy = 0.0;
                interval = setInterval(updateTimeAndEnergy, 100);
                StartMeterCheckBtn.innerText = 'Стоп⛔️';
                StartMeterCheckBtn.style["background-color"] = "red";
                document.getElementById('resultMeterCheck').style["display"] = "none";
                writeBtn.style["display"] = "none";
            }
        } else {
            clearInterval(interval);
            StartMeterCheckBtn.innerText = 'Старт▶';
            StartMeterCheckBtn.style["background-color"] = "#77dd77";
            calcMeterAccuracyAndShowRes(e);
        }
    }
}

let timer = document.getElementById('timer');
let resetBtn = document.getElementById('resetMeterCheck');
let calcEnergy = document.getElementById('CalcEnergy');
let calcPower = document.getElementById('CalcPower');
let power;
let seconds = 0.0;
let minutes = 0;
let hours = 0;
let energy = 0.0;
let interval;

function updateTimeAndEnergy() {
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
    updatePower();
    updateEnergy();
}

function updatePower() {
    power = voltageMeterCheck.value * currentMeterCheck.value;
    if (threePhaseMeterCheckBox.checked) {
        power += voltageMeterCheck_B.value * currentMeterCheck_B.value + 
        voltageMeterCheck_C.value * currentMeterCheck_C.value;
    }
    power *= сurrentTransformerTransformationRatio.value * powerFactor.value;
    calcPower.textContent = 'Pрасч = ' +  power.toFixed(1).toString() + ' Вт;  ';
}

function updateEnergy() {
    energy += power / 1000 * (hours + minutes / 60 + seconds / 3600);
    calcEnergy.textContent = 'Wрасч = ' +  energy.toFixed(1).toString() + ' кВт*ч';
}

resetBtn.addEventListener('click', () => {
    clearInterval(interval);
    seconds = 0.0;
    minutes = 0;
    hours = 0;
    power = 0.0;
    calcPower.textContent = 'Pрасч = ' +  power.toFixed(1).toString() + ' Вт;  ';
    energy = 0.0;
    calcEnergy.textContent ='Wрасч = ' +  energy.toFixed(1).toString() + ' кВт*ч';
    timer.textContent = '00:00:00.0';
    StartMeterCheckBtn.innerText = 'Старт▶';
    StartMeterCheckBtn.style["background-color"] = "#77dd77";
    document.getElementById('resultMeterCheck').style["display"] = "none";
});


powerFactor.addEventListener("change", calcMeterAccuracy);
сurrentTransformerTransformationRatio.addEventListener("change", calcMeterAccuracy);
voltageMeterCheck.addEventListener("change", calcMeterAccuracy);
currentMeterCheck.addEventListener("change", calcMeterAccuracy);
voltageMeterCheck_B.addEventListener("change", calcMeterAccuracy);
currentMeterCheck_B.addEventListener("change", calcMeterAccuracy);
voltageMeterCheck_C.addEventListener("change", calcMeterAccuracy);
currentMeterCheck_C.addEventListener("change", calcMeterAccuracy);
impsMeterCheck.addEventListener("change", calcMeterAccuracy);
impNumMeterCheck.addEventListener("change", calcMeterAccuracy);

function calcMeterAccuracy(e) {
    updatePower();
    const realPower = impNumMeterCheck.value / impsMeterCheck.value / (hours + minutes / 60 + seconds / 3600) * 1000;
    const accuracy = Math.abs(realPower - power) / power * 100;
    document.getElementById('RealMeterPower').textContent = 'Pфакт = ' + realPower.toFixed(1).toString() + ' Вт;';
    document.getElementById('MeterAccuracy').textContent = 'Погрешность = ' + accuracy.toFixed(1).toString() + ' %';
}

function calcMeterAccuracyAndShowRes(e) {
    /*if (seconds == 0.0 && minutes == 0 && hours == 0.0) {
        alert('Нажмите \"Старт\"');
    }*/
    if (voltageMeterCheck.value != '' && currentMeterCheck.value != '' && impNumMeterCheck.value != '' && impsMeterCheck.value != '') {
        if (!threePhaseMeterCheckBox.checked ||
            (voltageMeterCheck_B.value != '' && currentMeterCheck_B.value != '' && voltageMeterCheck_C.value != '' && currentMeterCheck_C.value != '')) {
            e.preventDefault();
            document.getElementById('resultMeterCheck').style["display"] = "";
            writeBtn.style["display"] = "";
        }
    }
    clearInterval(interval);
    StartMeterCheckBtn.innerText = 'Старт▶';
    StartMeterCheckBtn.style["background-color"] = "#77dd77";
    calcMeterAccuracy(e);
}

function clearALL() {
/** добавить очистку всех полей
 */
    writeBtn.style.display = "none";
}

const writeBtn = document.getElementById("copyResultButton");
writeBtn.addEventListener("click", copyResult);

function copyResult(e) {
    e.preventDefault();
    /*//для десктопов
    const writeBtn = document.getElementById('buttonId');
    const inputEl = document.querySelector('.output');
    const inputValue = inputEl.innerText;
    if (inputValue) {
        navigator.clipboard.writeText(inputValue)
        .then(() => {
            if (writeBtn.innerText !== 'Скопировано!') {
            const originalText = writeBtn.innerText;
            writeBtn.innerText = 'Скопировано!';
            setTimeout(() => {
                writeBtn.innerText = originalText;
            }, 1500);
            }
        })
        .catch(err => {
            console.log('Something went wrong', err);
        })
    }*/
    var inp = document.createElement('input')
    if (document.MeterCheck.style["display"] == "") {
        var now = new Date();
        inp.value = now + ';    \n\r\
Номер ИПУ: ' + document.getElementById('SMDSerialNumMeterCheck').value + ';    \n\r\
Время: ' + timer.textContent + ';    \n\r\
Ктт = ' + сurrentTransformerTransformationRatio.value + ' о.е.;    \n\r\
U1 = ' + voltageMeterCheck.value.toString() + ' В;    \n\r\
I1 = ' + currentMeterCheck.value.toString() + ' A;    \n\r\
U2 = ' + voltageMeterCheck_B.value.toString() + ' В;    \n\r\
I2 = ' + currentMeterCheck_B.value.toString() + ' A;    \n\r\
U3 = ' + voltageMeterCheck_C.value.toString() + ' В;    \n\r\
I3 = ' + currentMeterCheck_C.value.toString() + ' A;    \n\r'
+ calcPower.textContent + ';    \n\r'
+ calcEnergy.textContent + ';    \n\r\
A = ' + impsMeterCheck.value.toString() + ' имп/кВ*ч;   \n\r\
n = ' + impNumMeterCheck.value.toString() + ' имп;    \n\r'
+ document.getElementById('RealMeterPower').textContent + '    \n\r'
+ document.getElementById('MeterAccuracy').textContent;
    }
    document.body.appendChild(inp)
    inp.select()
    if (document.execCommand('copy')) {
        if (writeBtn.innerText !== 'Скопировано!') {
            const originalText = writeBtn.innerText;
            writeBtn.innerText = 'Скопировано!';
            setTimeout(() => {
                writeBtn.innerText = originalText;
            }, 1500);
        }
    } else {
        console.log("Failed...")
    }
    document.body.removeChild(inp)
}