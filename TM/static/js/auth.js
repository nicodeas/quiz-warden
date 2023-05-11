const loginUser = async () => {
  const username = document.getElementById("username");
  const password = document.getElementById("password");

  const credentials = {
    username: username.value,
    password: password.value,
  };
  // reset input values
  username.value = "";
  password.value = "";

  try {
    await fetch("http://localhost:8000/api/login", {
      headers: {
        Accept: "application/json",
        "Content-Type": "application/json",
      },
      method: "POST",
      body: JSON.stringify(credentials),
    })
    window.location.href = "http://localhost:8000";
  } catch (err) {
    console.log(err);
  }
};

const logoutUser = async (e) => {
  try {
    await fetch("http://localhost:8000/api/logout", {
    })
    window.location.href = "http://localhost:8000/login";
  } catch (err) {
    console.log(err);
  }
};