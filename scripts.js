function SendCommand(event, command) {
    localStorage.setItem("login", document.getElementById('login').value);
    localStorage.setItem("password", document.getElementById('password').value);

    if (event.key === "Enter") {
        fetch('/Command_' + document.getElementById('login').value + "_" + document.getElementById('password').value + "_" + btoa(command), {
            method: 'GET'
        }).then((responce) => {
            if (responce.ok) {
                fetch('res.txt', {
                    method: 'GET'
                }).then((responce) => {
                    responce.text().then((txt) => {
                        console.log(txt);

                        let resVec = txt.split("\n");
                        let output = document.querySelector("#output-list");
                        
                        let responce = '<div class="output__result">';
                        resVec.forEach((elem) => {
                            responce += elem;
                            responce += '<br>';
                        });
                        responce += '</div>';
                        output.innerHTML = '<div class="output"><div class="output__command">' + command + '</div>' + responce + '</div>' + output.innerHTML;
                    });
                });
            };
        });
    }
}

function ClearAll() {
    document.querySelector("#output-list").innerHTML = "";
}