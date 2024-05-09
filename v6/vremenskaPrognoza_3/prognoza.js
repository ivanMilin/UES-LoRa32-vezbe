function api_call()
{ 
    console.log(document.getElementById("lokacija").value);
    var url = "https://api.openweathermap.org/data/2.5/weather?q="+document.getElementById("lokacija").value+"&appid=c8fbcb3348a6bb150b912197549a956c&units=metric";
    console.log(url);
    
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() 
    {
        if (this.readyState == 4 && this.status == 200) 
        {
            console.log(xhttp.responseText);
            var obj = JSON.parse(xhttp.responseText);
            console.log(obj);
            document.getElementById("lokalitet").innerHTML = obj["name"] + ", " + obj["sys"]["country"];
            document.getElementById("temperatura").innerHTML = obj["main"]["temp"] + " &#x2103;";
            document.getElementById("vlaznost").innerHTML = obj["main"]["pressure"] + " mbar";
            document.getElementById("pritisak").innerHTML = obj["main"]["humidity"] + " %";
            var iconURL = "https://openweathermap.org/img/wn/" + obj["weather"][0]["icon"] + "@2x.png"

            document.getElementById("ikonica").src = iconURL;
            document.getElementById("weather_status").innerHTML = obj["weather"][0]["description"];
            
            document.getElementById("vremenski_podaci").style.display="block";
            document.getElementById("unos_lokacije").reset();
        }
    };
    xhttp.open("GET", url, true);
    xhttp.send();
}