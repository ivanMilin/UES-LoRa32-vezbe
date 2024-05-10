var ctx =
    document.getElementById("myChart").getContext("2d");
var myChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: ['Jan', 'Feb', 'Mar', 'Apr', 'Maj',
            'Jun', 'Jul'],
        datasets: [{
            label: 'Primer grafika',
            data: [65, 59, 80, 81, 56, 55, 40],
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