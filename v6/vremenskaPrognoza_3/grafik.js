var ctx = document.getElementById("myChart").getContext("2d");

var url = "https://api.openweathermap.org/data/2.5/forecast?q=Novi%20Sad&appid=c8fbcb3348a6bb150b912197549a956c&units=metric";

console.log(url);
    
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() 
{
    if (this.readyState == 4 && this.status == 200) 
    {
        console.log(xhttp.responseText);
        var obj = JSON.parse(xhttp.responseText);
        console.log(obj);
        var monday = obj["list"][0]["main"]["feels_like"];
        var tuesday = obj["list"][8]["main"]["feels_like"];
        var wednesday = obj["list"][16]["main"]["feels_like"];
        var truesday = obj["list"][24]["main"]["feels_like"];
        var friday = obj["list"][32]["main"]["feels_like"];
        
        var myChart = new Chart(ctx, {
            type: 'line',
            data: 
            {
                labels: ['Mon', 'Tue', 'Wed', 'Thr', 'Fri'],
                datasets: [{
                    label: 'Primer grafika',
                    data: [monday, tuesday, wednesday, truesday, friday],
                    fill: true,
                    borderColor: 'rgb(75, 192, 192)',
                    tension: 0.1
                }]
            },
            options: {
                responsive: false,
                maintainAspectRatio: false
            }
        });
    }
};
xhttp.open("GET", url, true);
xhttp.send();


