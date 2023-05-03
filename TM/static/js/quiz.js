let question = "";
choices = [];
let currentQuestionIndex = 1;

const getQuestion = async (previousQuestionIndex) => {
  document.getElementById("question-result").innerHTML = "";
  fetch("/api/question?number=" + currentQuestionIndex, {
    method: "GET",
    headers: {
      "Content-Type": "application/json",
    },
  })
    .then((response) => {
      if (!response.ok) {
        currentQuestionIndex = previousQuestionIndex;
        throw new Error("An error occurred fetching the question!");
      }
      return response.json();
    })
    .then((data) => {
      question = data.question;
      choices = data.choices;
      updateQuiz();
    });
};

const updateQuiz = () => {
  const choicesContainer = document.getElementById("quiz-choices");
  // Reset the container to blank
  choicesContainer.innerHTML = "";
  document.getElementById("quiz-question").innerHTML = question;

  // create multichoice fields
  choices.forEach((choice, idx) => {
    const radio = document.createElement("input");
    radio.type = "radio";
    radio.name = "option";
    radio.value = choice;
    radio.id = `choice-${idx}`;
    const label = document.createElement("label");
    label.innerHTML = choice;
    label.setAttribute("for", `choice-${idx}`);

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
  const questionAttemptsText = document.getElementById("question-attempts");

  const data = { answer: questionIndex, number: currentQuestionIndex };
  fetch("/api/question", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify(data),
  })
    .then((response) => response.json())
    .then((data) => {
      console.log(data);
      const result = data.correct ? "Correct!" : "Incorrect!";
      questionResultText.innerHTML = result;
      questionAttemptsText.innerHTML =
        "You have " + data.attempts + " Attempts!";
      console.log("ERTERT");
    })
    .catch((error) => {
      console.log(error);
      alert("An error occurred checking the answer!");
    });
};

const updateResultText = (result) => {};

document.getElementById("next-button").addEventListener("click", function (e) {
  e.preventDefault();
  const questionAttemptsText = document.getElementById("question-attempts");
  questionAttemptsText.innerHTML = "";

  let previousQuestionIndex = currentQuestionIndex;
  currentQuestionIndex++;
  getQuestion(previousQuestionIndex);
});

document.getElementById("back-button").addEventListener("click", function (e) {
  e.preventDefault();
  const questionAttemptsText = document.getElementById("question-attempts");
  questionAttemptsText.innerHTML = "";

  let previousQuestionIndex = currentQuestionIndex;
  currentQuestionIndex--;
  getQuestion(previousQuestionIndex);
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
