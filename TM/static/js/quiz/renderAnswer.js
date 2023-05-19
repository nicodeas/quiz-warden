export const renderAnswer = (data, type) => {
  const answerEl = document.getElementById("answer-container");
  if (type === "IMAGE") {
    answerEl.innerHTML = `<img src="data:image/png;base64, ${data.answer}" />`;
  } else {
    answerEl.innerHTML = `<p>The answer is: <span id="correct-answer"></span></p>`;
    document.getElementById("correct-answer").innerText = atob(data.answer);
  }
};
