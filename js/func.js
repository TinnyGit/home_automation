let btn = document.getElementById('btn1');
let slider = document.getElementById('myRange');

slider.oninput = function(){
	btn.innerText = slider.value;
}
