import React, { Component } from 'react'
import PropTypes from 'prop-types'

import Row from 'react-bootstrap/Row'
import Col from 'react-bootstrap/Col'
import Button from 'react-bootstrap/Button'
import Form from 'react-bootstrap/Form'

import './LEDTextSelector.css'

class LEDTextSelector extends Component {
  constructor (props) {
    super(props)

    this.state = { text: '' }

    this.pushSignText = this.pushSignText.bind(this)
  }

  pushSignText () {
    if (this.state.text === '') { return alert('Error: No text selected') }

    try {
      fetch('/api/updatetext', {
        method: 'POST',
        mode: 'no-cors',
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: 'text=' + encodeURIComponent(this.state.text)
      })
    } catch (err) {
      return alert('Error: ' + err)
    }
  }

  render () {
    const Options = this.props.options.map((option, idx) => {
      return (
        <Form.Check key={idx} type='radio' id={'LEDTextOption' + idx} name='LEDTextSelector' label={option} onClick={() => this.setState({ text: option })} />
      )
    })

    return (
      <>
        <Row className="spacious">
          <Col>
            Please select the text for display on the LED sign:
          </Col>
        </Row>
        <Row className="spacious">
          <Col>
            {Options}
          </Col>
        </Row>
        <Row className="spacious">
          <Col>
            Currently selected: {this.state.text}<br />
          </Col>
        </Row>
        <Row className="spacious">
          <Col>
            <Button onClick={() => this.pushSignText()}>Update LED Sign</Button>
          </Col>
        </Row>
        <Row className="spacious">
          <Col>
          </Col>
        </Row>
      </>
    )
  }
}

LEDTextSelector.propTypes = {
  options: PropTypes.array.isRequired
}

LEDTextSelector.defaultProps = {}

export default LEDTextSelector
