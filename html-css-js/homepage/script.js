// Scroll to section in page when it's id is selected from the nav bar, offsetting the height of the header
const headerHeight = document.querySelector('header').offsetHeight // Offset header height
document.querySelectorAll('nav a').forEach(link => {
    link.addEventListener('click', event => {
      event.preventDefault() // Prevent default behavior
      const targetId = link.getAttribute('href') // Get the target section id
      const targetSection = document.querySelector(targetId) // Find the target section
      targetSection.scrollIntoView({
        behavior: 'smooth',
        block: 'start',
      })
    })
})

// Move dislike button to a random location whenever mouse hovers over it
const dislikeButton = document.getElementById("dislikeButton")
dislikeButton.onmouseover = function() {
  dislikeButton.style.position = "absolute"
  dislikeButton.style.left = `${Math.ceil(Math.random() * 90)}%`
  dislikeButton.style.top = `${Math.ceil(Math.random() * 90)}%`
}

// Increase like count when like button clicked
document.getElementById('likeButton').addEventListener('click', (event) => {
  const button = event.target
  button.style.backgroundColor = '#34b290'
  let message = button.parentElement.querySelector('p.message')
  message.innerHTML = '<b>420</b> Likes <b>0</b> Dislikes<br>Thank you!'
})

// Randomly change colors of HTML elements when executed
function rainbow(){
  const colors = ['#b23434','#b25334','#b27334','#b29234','#b2b234','#92b234','#73b234','#34b273','	#34b2b2','#3492b2','#3453b2','#5334b2','#7334b2','#9234b2','#b234b2','#b23473']
  const colorfulElements = document.querySelectorAll('html, body, .banner, nav, button')

  colorfulElements.forEach(function(element){
    console.log(element)
    let index = Math.floor(Math.random() * colors.length)
    element.style.backgroundColor = colors[index]
  })
}

// Activates kickass game, allowing user to destroy current webpage
function destroy(){
  var s = document.createElement('script');s.type='text/javascript';document.body.appendChild(s);s.src='//hi.kickassapp.com/kickass.js'
}