import UIKit

class Draw1px: UIView {
    
    var count = 0;
    
    init() {
        super.init(frame: CGRect(x: 0, y: 0, width: 200, height: 200))
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func draw(_ rect: CGRect) {
        UIColor(red: 0, green: 0, blue: 0, alpha: 1).setFill()
        UIRectFill(bounds)

        switch (self.count) {
        case 1:
            UIColor(red: 255, green: 0, blue: 0, alpha: 1).setFill()
        case 2:
            UIColor(red: 0, green: 255, blue: 0, alpha: 1).setFill()
        case 3:
            UIColor(red: 0, green: 0, blue: 255, alpha: 1).setFill()
        default:
            UIColor(red: 0, green: 0, blue: 0, alpha: 1).setFill()
        }
        
        let rect = CGRect(x:100, y:100, width:1, height:1)
        UIRectFill(rect)
    
        count += 1;
        count = count % 4
    }
}
