let сurrentTransformerTransformationRatioCheck = document.getElementById('CurrentTransformerTransformationRatioCheck');

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
let PZEMinterval;
let timerInterval;
let ESPsurveyPeriod = 1000; // период опроса ESP
let StartMeterCheckBtn = document.getElementById('StartMeterCheck');
let resetBtn = document.getElementById('resetMeterCheck');

StartMeterCheckBtn.addEventListener('click', startMeterCheck);

function CheckConstMeterImpsNumInputs() {
    if (constMeterImpsNumCheck.value == ''
        || constMeterImpsNumCheck.value <= 0) {
            alert('A[имп/кВ*ч] должна быть больше 0');
            return false;
        }
    return true;
}

function CheckCurrentTransformerTransformationRatioInputs() {
    if (сurrentTransformerTransformationRatioCheck.value == ''
        || сurrentTransformerTransformationRatioCheck.value <= 0) {
            alert('Ктт должен быть больше 0');
            return false;
        }
    return true;
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
    if (CheckConstMeterImpsNumInputs()) {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "const_meter_imps_num?constMeterImpsNumVal="+constMeterImpsNumCheck.value, true);
        xhttp.responseType = "json";
        xhttp.send();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                console.log("sendConstMeterImpsNumCheck successful✔️\n\r");
            }
        };
        xhttp.onload = function () {
            ViewAllESPdata(xhttp.response);
        };
    }
};

сurrentTransformerTransformationRatioCheck.addEventListener("change", sendCurrentTransformerTransformationRatio);

function sendCurrentTransformerTransformationRatio() {
    if (CheckCurrentTransformerTransformationRatioInputs()) {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET",
            "current_transformer_transformation_ratio?сurrentTransformerTransformationRatio="+сurrentTransformerTransformationRatioCheck.value,
            true);
        xhttp.responseType = "json";
        xhttp.send();
        xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log("sendCurrentTransformerTransformationRatio successful✔️\n\r");
        }
        };
        xhttp.onload = function () {
            ViewAllESPdata(xhttp.response);
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
    if (StartMeterCheckBtn.innerText == 'Старт▶') {
        if (CheckConstMeterImpsNumInputs()
            && CheckCurrentTransformerTransformationRatioInputs()) {
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
        console.log(" джём, пока придут последние значения с pzem");
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
        var now = new Date();
        inp.value = now + ';    \n\r\
Номер ИПУ: ' + document.getElementById('SMDSerialNumMeterCheck').value + ';    \n\r\
Время: ' + timer.textContent + ';    \n\r\
Ктт = ' + сurrentTransformerTransformationRatioCheck.value + ' о.е.;    \n\r\
U1 = ' + pzemVoltageMeterCheck1.value.toString() + ' В;    \n\r\
I1 = ' + pzemCurrentMeterCheck1.value.toString() + ' A;    \n\r\
U2 = ' + pzemVoltageMeterCheck2.value.toString() + ' В;    \n\r\
I2 = ' + pzemCurrentMeterCheck2.value.toString() + ' A;    \n\r\
U3 = ' + pzemVoltageMeterCheck3.value.toString() + ' В;    \n\r\
I3 = ' + pzemCurrentMeterCheck3.value.toString() + ' A;    \n\r'
+ calcMeterPower.textContent + ';    \n\r\
A = ' + constMeterImpsNumCheck.value.toString() + ' имп/кВ*ч;   \n\r\
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