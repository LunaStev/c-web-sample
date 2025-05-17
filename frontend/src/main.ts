fetch('/api/hello')
    .then(res => res.text())
    .then(text => {
        document.getElementById("response")!.innerText = text;
    });
