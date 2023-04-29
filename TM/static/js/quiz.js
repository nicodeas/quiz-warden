let question = "";
choices = [];
let currentRound = 1;

const getQuestion = async () => {
  document.getElementById("question-result").innerHTML = "";
  fetch("/api/question?number=" + currentRound, {
    method: "GET",
    headers: {
      "Content-Type": "application/json",
    },
  })
    .then((response) => response.json())
    .then((data) => {
      console.log(data);
      console.log(data.question);
      question = data.question;
      choices = data.choices;
      updateQuiz();
    })
    .catch((error) => {
      console.log(error);
      alert("An error occurred fetching the question!");
    });
};

const updateQuiz = () => {
  const choicesContainer = document.getElementById("quiz-choices");
  // Reset the container to blank
  choicesContainer.innerHTML = "";
  document.getElementById("quiz-question").innerHTML = question;

  // create multichoice fields
  choices.forEach((choice) => {
    const radio = document.createElement("input");
    radio.type = "radio";
    radio.name = "option";
    radio.value = choice;
    const label = document.createElement("label");
    label.innerHTML = choice;
    const choiceContainer = document.createElement("div");

    choiceContainer.appendChild(radio);
    choiceContainer.appendChild(label);
    choicesContainer.appendChild(choiceContainer);
  });
};

getQuestion();

// Makes a POST request with the index of the chosen answer
const checkQuestion = async (questionIndex) => {
  const questionResultText = document.getElementById("question-result");
  const data = { answer: questionIndex };
  fetch("/api/question", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify(data),
  })
    .then((response) => response.json())
    .then((data) => {
      console.log(data.correct);
      const result = data.correct ? "Correct!" : "Incorrect!";
      questionResultText.innerHTML = result;
    })
    .catch((error) => {
      console.log(error);
      alert("An error occurred checking the answer!");
    });
};

const updateResultText = (result) => {};

document.getElementById("next-button").addEventListener("click", function (e) {
  e.preventDefault();
  currentRound++;
  getQuestion();
});

document.getElementById("back-button").addEventListener("click", function (e) {
  e.preventDefault();
  currentRound--;
  getQuestion();
});

document.getElementById("check-button").addEventListener("click", function (e) {
  e.preventDefault();
  const options = document.getElementsByName("option");
  let checkedQuestionIndex = -1;
  for (let i = 0; i < options.length; i++) {
    if (options[i].checked) {
      checkedQuestionIndex = i;
      break;
    }
  }
  if (checkedQuestionIndex === -1) {
    alert("Please select an answer!");
    return;
  }
  checkQuestion(checkedQuestionIndex);
});
