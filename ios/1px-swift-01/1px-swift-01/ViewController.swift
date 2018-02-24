import UIKit

class ViewController: UIViewController {
        
    @IBOutlet weak var view_container: UIView!
    var draw1pxview : Draw1px!
    var timer: Timer!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        draw1pxview = Draw1px()
        self.view_container.addSubview(draw1pxview)
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(true)
        timer = Timer.scheduledTimer(
            timeInterval: 0.25,
            target: self,
            selector: #selector(self.update),
            userInfo: nil,
            repeats: true)
        timer.fire()
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(true)
        timer.invalidate()
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    @objc func update(tm: Timer) {
        draw1pxview.setNeedsDisplay()
    }
}

