var Counter = -1;

function SendCommand(event, command) {
    if (event.keyCode == '38') {
        var commands = localStorage.getItem("commands");
        console.log(commands);
        if (commands !== null)
        {
            var commandsVec = commands.split("\n");
            
            if (Counter < commandsVec.length - 2) {
                Counter += 1;
                document.querySelector("#inputPole").value = commandsVec[Counter];
            }
        }
    }

    if (event.keyCode == '40' && Counter > -1) {
        Counter -= 1;

        if (Counter != -1)
            document.querySelector("#inputPole").value = localStorage.getItem("commands").split("\n")[Counter];
        else
            document.querySelector("#inputPole").value = "";
    }

    if (event.key === "Enter") {
        var isAllFilled = true;
        if (command === null || command.length == 0) {
            document.querySelector('#inputPole').classList.add('input__red');
            isAllFilled = false;
        }
        else {
            document.querySelector('#inputPole').classList.remove('input__red');
        }
                
        if (document.querySelector('#login').value == "") {
            document.querySelector('#login').classList.add('input__red');
            isAllFilled = false
        }
        else {
            document.querySelector('#login').classList.remove('input__red');
        }

        if (document.querySelector('#password').value == "") {
            document.querySelector('#password').classList.add('input__red');
            isAllFilled = false
        }
        else {
            document.querySelector('#password').classList.remove('input__red');
        }

        if (!isAllFilled) {
            return;
        }

        Counter = -1;
        localStorage.setItem("login", document.getElementById('login').value);
        localStorage.setItem("password", document.getElementById('password').value);

        if (localStorage.getItem("commands") === null) {
            localStorage.setItem("commands", command + "\n");
        }
        else {
            localStorage.setItem("commands", command + "\n" + localStorage.getItem("commands"));
        }
        
        console.log(localStorage.getItem("commands"));
        document.querySelector("#inputPole").value = "";

        fetch('/Command_' + document.getElementById('login').value + "_" + document.getElementById('password').value + "_" + btoa(command), {
            method: 'GET'
        }).then((responce) => {
            if (responce.ok) {
                fetch('/CommandRes', {
                    method: 'GET'
                }).then((responce) => {
                    responce.text().then((txt) => {
                        console.log(txt);
                        console.log(decodeURIComponent(txt));
                        
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