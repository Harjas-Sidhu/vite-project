function App() {

  const localhost = window.location.href;

  function sendRequest(){
    fetch('https://'+localhost+':8000', {method: "GET"})
    .then((response) => {
        if(!response.ok){
          console.log(response.status);
        }
    })
    .catch((error) => {
      console.log("fetch error: ", error);
    })
  }

  function handleClick() {

    if(document.getElementById("table")){
      document.getElementsByTagName("body")[0].removeChild(document.getElementById("table"));
    }

    const table = document.createElement("table");
    table.id = "table";

    let lines = [];

    sendRequest();

    fetch(localhost+'/src/assets/output/stats-roundrobin.csv')
      .then(data => { return data.text() })
      .then(text => { return text.toString().split("\n"); })
      .then(text => { let arr = text; lines = lines.concat(arr); })
      .then(() => {
        for (let i = 0; i < lines.length - 1; i++) {

          const elements = lines[i].split(',');
          const row = document.createElement("tr");
          row.classList.add(`row-${i + 1}`);

          if (i == 0) {
            for (let j = 0; j < elements.length; j++) {
              const th = document.createElement("th");
              th.classList.add(`table-header-${j + 1}`);
              th.innerHTML = elements[j].toString();
              row.appendChild(th);
            }
          }

          else {
            for (let j = 0; j < elements.length; j++) {
              const td = document.createElement("td");
              td.classList.add(`${i + 1}-${j + 1}`);
              td.innerHTML = Number.parseFloat(elements[j]);
              row.appendChild(td);
            }
          }

          table.appendChild(row);
        }
      });

    fetch(localhost+':5173/src/assets/output/extra-info.txt')
      .then(text => { return text.text() })
      .then(text => { return text.split(","); })
      .then(text => { 
        table.style.height = `${(Number.parseInt(text[0])+1)*8}vh`;
        document.getElementById('para').innerHTML = `Time Quantum = ${Number.parseFloat(text[1])}`;
      })
      .then(() => {document.getElementsByTagName("body")[0].appendChild(table);})
  }

  return (
    <>
      <button onClick={handleClick}>Click Me!</button>
      <p id="para"></p>
    </>
  )
}

export default App
