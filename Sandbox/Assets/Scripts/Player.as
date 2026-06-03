class Player : IEntity
{
    Entity self;

    void Start() {
        Print("Hellope");
    }

    void Update(float dt) {
        float xpos = self.get_position_x();
        self.set_position(xpos + 0.005, 0);
    }
}