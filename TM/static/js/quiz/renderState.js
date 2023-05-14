export const renderState = async (state) => {
  const { currentQuestion, maxQuestions, completed, activeQBs } = state;

  const currentQuestionElement = document.getElementById("current-question");
  const maxQuestionsElement = document.getElementById("max-questions");

  const pythonBankStatusElement = document.getElementById("python-bank-status");
  const cBankStatusElement = document.getElementById("c-bank-status");

  const pythonBankStatus = activeQBs.includes("Python");
  const cBankStatus = activeQBs.includes("C");

  pythonBankStatusElement.innerHTML = pythonBankStatus ? "Online" : "Offline";
  pythonBankStatusElement.style.color = pythonBankStatus ? "green" : "red";
  cBankStatusElement.innerHTML = cBankStatus ? "Online" : "Offline";
  cBankStatusElement.style.color = cBankStatus ? "green" : "red";

  currentQuestionElement.innerHTML = currentQuestion;
  maxQuestionsElement.innerHTML = maxQuestions;

  // conditionally remove navigation buttons
  const nextButton = document.getElementById("next-button");
  const backButton = document.getElementById("back-button");
  if (currentQuestion == state.maxQuestions) {
    nextButton.style.display = "none";
  }

  if (currentQuestion == 1) {
    backButton.style.display = "none";
  }
};
